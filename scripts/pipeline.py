#!/usr/bin/env python3
"""Run stale HomeNexus cross-build stages, deploy, and optionally launch."""

from __future__ import annotations

import argparse
import os
import re
import shlex
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Mapping, Optional, Sequence, Tuple


SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR = SCRIPT_DIR.parent
ARTIFACTS_DIR = ROOT_DIR / "artifacts"

SYSROOT_PATHS = ("docker/Dockerfile.sysroot",)
SDK_PATHS = (
    "docker/Dockerfile.sdk",
    "cmake/toolchain.cmake",
    "cmake/opencvtoolchain.cmake",
    "tools/sysroot-relativelinks.py",
)

STATE_KEYS = (
    "SYSROOT_COMMIT",
    "SDK_COMMIT",
    "SDK_BUILD_OPENCV",
    "APP_COMMIT",
    "APP_BUILD_TYPE",
)
STATE_LINE = re.compile(r'^([A-Z_]+)="([^"\n]*)"$')


class PipelineError(RuntimeError):
    """Report an expected pipeline failure without a traceback."""


class PipelineArgumentParser(argparse.ArgumentParser):
    """Report argument errors with the pipeline's exit-code convention."""

    def error(self, message: str) -> None:
        self.exit(1, f"Error: {message}\n")


@dataclass(frozen=True)
class Config:
    """Resolved pipeline configuration."""

    state_file: Path
    build_opencv: str
    build_type: str
    app_dir: str
    app_binary_name: str
    pi_user: str
    pi_host: str
    pi_port: str
    sysroot_image: str
    sdk_image: str

    @property
    def ssh_target(self) -> str:
        return f"{self.pi_user}@{self.pi_host}"

    @property
    def sysroot_tarball(self) -> Path:
        return ARTIFACTS_DIR / "rasp.tar.gz"

    @property
    def app_binary(self) -> Path:
        return ARTIFACTS_DIR / self.app_binary_name

    @property
    def qt_pi_tarball(self) -> Path:
        return ARTIFACTS_DIR / "qt-pi-binaries.tar.gz"


@dataclass
class Stage:
    """A pipeline stage decision and its persisted metadata."""

    name: str
    run: bool = False
    reason: str = ""


def env(name: str, default: str) -> str:
    """Return a non-empty environment value or its default."""
    return os.environ.get(name) or default


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    """Parse command-line arguments while preserving the Bash CLI."""
    parser = PipelineArgumentParser(
        description=(
            "Run the three-stage cross-build (sysroot -> SDK -> app), rebuilding "
            "only stale stages, then optionally deploy and launch on the Raspberry Pi."
        )
    )
    parser.add_argument(
        "--deploy", action="store_true", help="deploy after a successful build"
    )
    parser.add_argument(
        "--launch",
        action="store_true",
        help="launch the app as a systemd service after deployment",
    )
    parser.add_argument(
        "--force",
        nargs="?",
        choices=("sysroot", "sdk", "app", "all"),
        action="append",
        dest="force",
        metavar="STAGE",
        help="force a rebuild; use --force=STAGE or bare --force for all",
    )
    for stage in ("sysroot", "sdk", "app", "all"):
        parser.add_argument(
            f"--force-{stage}",
            action="append_const",
            const=stage,
            dest="force",
            help=argparse.SUPPRESS,
        )
    parser.add_argument(
        "--build-opencv",
        choices=("ON", "OFF"),
        default=env("BUILD_OPENCV", "OFF"),
        help="OpenCV SDK build flag (default: %(default)s)",
    )
    parser.add_argument(
        "--build-type",
        default=env("BUILD_TYPE", "Release"),
        metavar="TYPE",
        help="application build type (default: %(default)s)",
    )
    parser.add_argument(
        "--dry-run", action="store_true", help="print the plan without executing it"
    )
    parser.add_argument(
        "-y",
        "--yes",
        action="store_true",
        help="do not prompt before deploying",
    )

    normalized_argv = []
    valid_force_values = {"sysroot", "sdk", "app", "all"}
    for argument in argv:
        if argument == "--force":
            normalized_argv.append("--force-all")
        elif argument.startswith("--force="):
            force_value = argument.removeprefix("--force=")
            if force_value not in valid_force_values:
                parser.error(
                    f"Invalid --force value: {force_value} "
                    "(use sysroot|sdk|app|all)"
                )
            normalized_argv.append(f"--force-{force_value}")
        else:
            normalized_argv.append(argument)

    args = parser.parse_args(normalized_argv)
    if args.force is None:
        args.force = []
    if args.build_opencv not in ("ON", "OFF"):
        parser.error("BUILD_OPENCV must be ON or OFF")
    if not args.build_type:
        parser.error("--build-type must not be empty")
    if args.launch and not args.deploy:
        parser.error("--launch requires --deploy")
    return args


def make_config(args: argparse.Namespace) -> Config:
    """Resolve environment-backed configuration."""
    state_file = Path(
        env("STATE_FILE", str(ARTIFACTS_DIR / ".pipeline-state"))
    ).expanduser().resolve()
    return Config(
        state_file=state_file,
        build_opencv=args.build_opencv,
        build_type=args.build_type,
        app_dir=env("APP_DIR", "app/HomeNexus"),
        app_binary_name=env("APP_BINARY_NAME", "appHomeNexus"),
        pi_user=env("PI_USER", "strjo2"),
        pi_host=env("PI_HOST", "192.168.178.36"),
        pi_port=env("PI_PORT", "22"),
        sysroot_image=env("SYSROOT_IMAGE", "qt-sysroot:latest"),
        sdk_image=env("SDK_IMAGE", "qtcrossbuild:latest"),
    )


def run(
    command: Sequence[str],
    *,
    check: bool = True,
    capture_output: bool = False,
    environment: Optional[Mapping[str, str]] = None,
) -> subprocess.CompletedProcess[str]:
    """Run a command from the repository root."""
    return subprocess.run(
        command,
        cwd=ROOT_DIR,
        check=check,
        capture_output=capture_output,
        text=True,
        env=environment,
    )


def command_exists(command: str) -> bool:
    """Return whether a command is available in PATH."""
    return shutil.which(command) is not None


def require_executable(path: Path) -> None:
    """Require an executable file."""
    if not path.is_file() or not os.access(path, os.X_OK):
        raise PipelineError(f"{path} not found or not executable")


def preflight(deploy: bool) -> None:
    """Validate required commands and stage scripts before side effects."""
    if not command_exists("docker"):
        raise PipelineError("docker is not installed or not in PATH")
    for script_name in ("build-sysroot.sh", "build-sdk-image.sh", "build-app.sh"):
        require_executable(SCRIPT_DIR / script_name)
    if deploy:
        if not command_exists("ssh"):
            raise PipelineError("ssh is not installed or not in PATH")
        require_executable(SCRIPT_DIR / "deploy.sh")


def git_head() -> Tuple[bool, str]:
    """Return repository availability and the current commit."""
    result = run(
        ("git", "rev-parse", "HEAD"), check=False, capture_output=True
    )
    if result.returncode == 0:
        return True, result.stdout.strip()
    print(
        "Warning: not a git repository with commits; stage staleness relies "
        "on image/artifact presence only",
        file=sys.stderr,
    )
    return False, ""


def git_changed(git_ok: bool, commit: str, paths: Sequence[str]) -> bool:
    """Return whether paths changed locally or since a recorded commit."""
    if not git_ok:
        return False
    status = run(
        ("git", "status", "--porcelain", "--", *paths), capture_output=True
    )
    if status.stdout:
        return True
    if not commit:
        return False
    commit_exists = run(
        ("git", "cat-file", "-e", f"{commit}^{{commit}}"),
        check=False,
        capture_output=True,
    )
    if commit_exists.returncode != 0:
        return False
    changed = run(
        ("git", "diff", "--name-only", commit, "HEAD", "--", *paths),
        capture_output=True,
    )
    return bool(changed.stdout)


def have_image(image: str) -> bool:
    """Return whether a Docker image exists locally."""
    result = run(
        ("docker", "image", "inspect", image),
        check=False,
        capture_output=True,
    )
    return result.returncode == 0


def load_state(path: Path) -> dict[str, str]:
    """Load the shell-compatible pipeline state file."""
    state = {key: "" for key in STATE_KEYS}
    if not path.is_file():
        return state
    try:
        lines = path.read_text(encoding="utf-8").splitlines()
    except OSError as error:
        raise PipelineError(f"cannot read state file {path}: {error}") from error
    for line in lines:
        match = STATE_LINE.fullmatch(line)
        if not match or match.group(1) not in state:
            raise PipelineError(f"invalid pipeline state line: {line!r}")
        state[match.group(1)] = match.group(2)
    return state


def save_state(path: Path, state: Mapping[str, str]) -> None:
    """Persist state in the format used by the previous Bash orchestrator."""
    for key in STATE_KEYS:
        if '"' in state[key] or "\n" in state[key]:
            raise PipelineError(f"state value for {key} contains invalid characters")
    try:
        path.parent.mkdir(parents=True, exist_ok=True)
        content = "".join(f'{key}="{state[key]}"\n' for key in STATE_KEYS)
        path.write_text(content, encoding="utf-8")
    except OSError as error:
        raise PipelineError(f"cannot write state file {path}: {error}") from error


def forced_stages(values: Sequence[str]) -> set[str]:
    """Expand repeated force arguments into stage names."""
    forced = set(values)
    if "all" in forced:
        return {"sysroot", "sdk", "app"}
    return forced


def decide_stages(
    config: Config,
    state: Mapping[str, str],
    git_ok: bool,
    forced: set[str],
) -> Tuple[Stage, Stage, Stage]:
    """Decide stale stages top-down so upstream rebuilds propagate."""
    sysroot = Stage("sysroot")
    if "sysroot" in forced:
        sysroot.run, sysroot.reason = True, "forced"
    elif not have_image(config.sysroot_image):
        sysroot.run = True
        sysroot.reason = f"image {config.sysroot_image} missing"
    elif not config.sysroot_tarball.is_file():
        sysroot.run, sysroot.reason = True, "artifact rasp.tar.gz missing"
    elif git_changed(git_ok, state["SYSROOT_COMMIT"], SYSROOT_PATHS):
        sysroot.run, sysroot.reason = True, "inputs changed"

    sdk = Stage("sdk")
    if sysroot.run:
        sdk.run, sdk.reason = True, "sysroot stage reran"
    elif "sdk" in forced:
        sdk.run, sdk.reason = True, "forced"
    elif not have_image(config.sdk_image):
        sdk.run = True
        sdk.reason = f"image {config.sdk_image} missing"
    elif (
        state["SDK_BUILD_OPENCV"]
        and state["SDK_BUILD_OPENCV"] != config.build_opencv
    ):
        sdk.run = True
        sdk.reason = (
            "BUILD_OPENCV changed "
            f"({state['SDK_BUILD_OPENCV']} -> {config.build_opencv})"
        )
    elif git_changed(git_ok, state["SDK_COMMIT"], SDK_PATHS):
        sdk.run, sdk.reason = True, "inputs changed"

    app = Stage("app")
    if sdk.run:
        app.run, app.reason = True, "sdk stage reran"
    elif "app" in forced:
        app.run, app.reason = True, "forced"
    elif not config.app_binary.is_file() or not config.qt_pi_tarball.is_file():
        app.run, app.reason = True, "app artifacts missing"
    elif state["APP_BUILD_TYPE"] and state["APP_BUILD_TYPE"] != config.build_type:
        app.run = True
        app.reason = (
            f"BUILD_TYPE changed ({state['APP_BUILD_TYPE']} -> {config.build_type})"
        )
    elif git_changed(
        git_ok, state["APP_COMMIT"], (config.app_dir, "docker/Dockerfile.app")
    ):
        app.run, app.reason = True, "inputs changed"

    return sysroot, sdk, app


def print_plan(
    stages: Sequence[Stage], config: Config, deploy: bool, launch: bool
) -> None:
    """Print the ordered build and deploy plan."""
    print("==============================================================")
    print(" Cross-build plan")
    print("==============================================================")
    for stage in stages:
        if stage.run:
            print(f"    {stage.name:<8} RUN   ({stage.reason})")
        else:
            print(f"    {stage.name:<8} skip")
    if deploy:
        print(f"    deploy   RUN   (target {config.ssh_target}:{config.pi_port})")
    if launch:
        print("    launch   RUN   (systemd service after deployment)")


def stage_environment(**updates: str) -> dict[str, str]:
    """Create a child environment with stage-specific overrides."""
    environment = os.environ.copy()
    environment.update(updates)
    return environment


def execute_stages(
    stages: Sequence[Stage],
    config: Config,
    state: dict[str, str],
    head_commit: str,
) -> None:
    """Execute planned stages and update in-memory state after each success."""
    sysroot, sdk, app = stages
    if sysroot.run:
        print("\n==> Stage 1: sysroot")
        run((str(SCRIPT_DIR / "build-sysroot.sh"),))
        state["SYSROOT_COMMIT"] = head_commit

    if sdk.run:
        print(f"\n==> Stage 2: SDK (BUILD_OPENCV={config.build_opencv})")
        run(
            (str(SCRIPT_DIR / "build-sdk-image.sh"),),
            environment=stage_environment(BUILD_OPENCV=config.build_opencv),
        )
        state["SDK_COMMIT"] = head_commit
        state["SDK_BUILD_OPENCV"] = config.build_opencv

    if app.run:
        print(f"\n==> Stage 3: app (BUILD_TYPE={config.build_type})")
        run(
            (str(SCRIPT_DIR / "build-app.sh"),),
            environment=stage_environment(
                APP_DIR=config.app_dir,
                BUILD_TYPE=config.build_type,
                APP_BINARY_NAME=config.app_binary_name,
            ),
        )
        state["APP_COMMIT"] = head_commit
        state["APP_BUILD_TYPE"] = config.build_type


def establish_state_baseline(
    state: dict[str, str], config: Config, head_commit: str
) -> None:
    """Record a baseline when existing images allowed a stage to be skipped."""
    if not head_commit:
        return
    if not state["SYSROOT_COMMIT"]:
        state["SYSROOT_COMMIT"] = head_commit
    if not state["SDK_COMMIT"]:
        state["SDK_COMMIT"] = head_commit
    if not state["SDK_BUILD_OPENCV"]:
        state["SDK_BUILD_OPENCV"] = config.build_opencv
    if not state["APP_COMMIT"]:
        state["APP_COMMIT"] = head_commit
    if not state["APP_BUILD_TYPE"]:
        state["APP_BUILD_TYPE"] = config.build_type


def deploy(config: Config, assume_yes: bool, launch: bool) -> None:
    """Verify artifacts and SSH reachability, then run the existing deploy script."""
    if not config.app_binary.is_file():
        raise PipelineError(
            f"App binary not found: {config.app_binary} (build the app stage first)"
        )
    if not config.qt_pi_tarball.is_file():
        raise PipelineError(
            "Qt runtime tarball not found: "
            f"{config.qt_pi_tarball} (build the app stage first)"
        )

    if not assume_yes and sys.stdin.isatty():
        answer = input(f"Deploy to {config.ssh_target}:{config.pi_port}? [y/N] ")
        if answer.lower() != "y":
            raise PipelineError("Deploy aborted by user")

    print(f"\n==> Checking Raspberry Pi reachability: {config.ssh_target}:{config.pi_port}")
    probe = run(
        (
            "ssh",
            "-o",
            "BatchMode=yes",
            "-o",
            "ConnectTimeout=5",
            "-p",
            config.pi_port,
            config.ssh_target,
            "true",
        ),
        check=False,
    )
    if probe.returncode != 0:
        raise PipelineError(
            f"Cannot reach {config.ssh_target}:{config.pi_port} over SSH "
            "(device offline, wrong host/port, or key auth not set up)"
        )

    print(f"==> Deploying to {config.ssh_target}")
    run(
        (str(SCRIPT_DIR / "deploy.sh"),),
        environment=stage_environment(
            PI_USER=config.pi_user,
            PI_HOST=config.pi_host,
            PI_PORT=config.pi_port,
            LAUNCH_APP="1" if launch else "0",
        ),
    )
    if launch:
        print("==> Deploy and launch finished successfully")
    else:
        print("==> Deploy finished successfully")


def main(argv: Optional[Sequence[str]] = None) -> int:
    """Run the cross-build and optional deployment workflow."""
    args = parse_args(argv if argv is not None else sys.argv[1:])
    config = make_config(args)
    try:
        preflight(args.deploy)
        git_ok, head_commit = git_head()
        state = load_state(config.state_file)
        stages = decide_stages(
            config, state, git_ok, forced_stages(args.force)
        )
        print_plan(stages, config, args.deploy, args.launch)
        if args.dry_run:
            print("==> Dry run: no stages executed")
            return 0

        execute_stages(stages, config, state, head_commit)
        establish_state_baseline(state, config, head_commit)
        save_state(config.state_file, state)
        if any(stage.run for stage in stages):
            print("==> Build finished successfully")
        else:
            print("==> Build up to date; nothing rebuilt")

        if args.deploy:
            deploy(config, args.yes, args.launch)
    except PipelineError as error:
        print(f"Error: {error}", file=sys.stderr)
        return 1
    except subprocess.CalledProcessError as error:
        print(
            f"Error: command failed with exit code {error.returncode}: "
            f"{shlex.join(error.cmd)}",
            file=sys.stderr,
        )
        return error.returncode or 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
