---
name: cross-compile-deploy
description: "Use when cross-building HomeNexus for the Raspberry Pi, running the sysroot/SDK/app Docker stages, rebuilding after an app-only C++/Qt/QML change, deploying the binary and Qt runtime to the Pi over SSH, or diagnosing cross-build, artifact-extraction, or deploy connectivity failures."
---

# HomeNexus cross-build & deploy

Orchestrate the three-stage Raspberry Pi cross-build and deployment, rebuilding
only the stages whose inputs changed. Use `scripts/pipeline.py`; it wraps the
existing `build-sysroot.sh` -> `build-sdk-image.sh` -> `build-app.sh` -> `deploy.sh`
scripts and never merges or bypasses them.

## Pipeline overview

| Stage | Script | Consumes | Produces |
|-------|--------|----------|----------|
| 1 sysroot | `build-sysroot.sh` | `docker/Dockerfile.sysroot` | image `qt-sysroot:latest`, `artifacts/rasp.tar.gz` |
| 2 SDK | `build-sdk-image.sh` | `Dockerfile.sdk`, `rasp.tar.gz`, `cmake/toolchain.cmake`, `cmake/opencvtoolchain.cmake`, `tools/sysroot-relativelinks.py`, `BUILD_OPENCV` | image `qtcrossbuild:latest` |
| 3 app | `build-app.sh` | `qtcrossbuild:latest`, `app/HomeNexus/**`, `Dockerfile.app`, `BUILD_TYPE` | `artifacts/appHomeNexus`, `artifacts/qt-pi-binaries.tar.gz`, optional `opencv-binaries.tar.gz` |
| deploy | `deploy.sh` | app binary, Qt tarball, optional `homenexus.ini`, fallback weather JSON | files installed on the Pi via `remote-setup.sh` |

Stages are strictly ordered: rerunning an upstream stage forces every stage below it.

## When each stage reruns

`pipeline.py` combines git change classification with Docker image / artifact
presence. It records the last-built commit and flags per stage in the gitignored
`artifacts/.pipeline-state`.

| Stage | Reruns when |
|-------|-------------|
| sysroot | `qt-sysroot:latest` or `rasp.tar.gz` missing; git changes touch `docker/Dockerfile.sysroot`; `--force=sysroot` |
| SDK | sysroot reran; `qtcrossbuild:latest` missing; git changes touch `Dockerfile.sdk` / `cmake/*.cmake` / `tools/sysroot-relativelinks.py`; `BUILD_OPENCV` differs from the recorded value; `--force=sdk` |
| app | SDK reran; git changes touch `app/HomeNexus/**` or `Dockerfile.app`; `appHomeNexus` or `qt-pi-binaries.tar.gz` missing; `BUILD_TYPE` differs from the recorded value; `--force=app` |

The change set is the union of uncommitted changes (`git status`) and commits
since the recorded per-stage commit (`git diff`). With no recorded state, a stage
is skipped when its image/artifact is already present. The sysroot base image is
tracked by git only; refresh its OS packages with `--force=sysroot`.

## Common scenarios

Preview the plan without building (start here when unsure):

```bash
python3 scripts/pipeline.py --dry-run
```

App-only C++/Qt/QML change — rebuilds the app stage only:

```bash
python3 scripts/pipeline.py
```

Build and deploy to the Pi (probes SSH reachability first):

```bash
python3 scripts/pipeline.py --deploy
```

Build, deploy, and launch HomeNexus as a systemd service:

```bash
python3 scripts/pipeline.py --deploy --launch
```

`--launch` requires `--deploy`. It installs and enables `homenexus.service`,
stops any manually launched `appHomeNexus` process, and verifies that the new
service is active. Subsequent boots start the service automatically.

Toolchain / SDK change (e.g. edited `Dockerfile.sdk` or a `cmake/*.cmake`):

```bash
python3 scripts/pipeline.py            # SDK + app rerun automatically (OpenCV OFF by default)
BUILD_OPENCV=ON python3 scripts/pipeline.py   # opt in to (re)build OpenCV in the SDK
```

Full rebuild from scratch:

```bash
python3 scripts/pipeline.py --force=all
```

Non-interactive deploy (CI or scripted):

```bash
python3 scripts/pipeline.py --deploy --yes
```

Override the target device inline:

```bash
PI_HOST=raspberrypi.local PI_USER=pi PI_PORT=22 python3 scripts/pipeline.py --deploy
```

## Deploy preflight

`--deploy` runs only after a successful build and verifies:

1. `artifacts/appHomeNexus` and `artifacts/qt-pi-binaries.tar.gz` exist.
2. The Pi answers a non-interactive SSH probe:
   `ssh -o BatchMode=yes -o ConnectTimeout=5 -p "$PI_PORT" "$PI_USER@$PI_HOST" true`.

If the probe fails, deployment aborts before any `scp` — check that the device is
powered on and on the network, the host/port are correct, and key-based auth is
configured. `deploy.sh` then performs its own `require_file` checks for config and
fallback data.

With `--launch`, `remote-setup.sh` additionally requires `systemctl` and `pkill`
on the target. Service installation and control use the existing explicit `sudo`
path. Plain `--deploy` continues to install files without starting or restarting
the application.

## Troubleshooting

- **`base image qtcrossbuild:latest not found`** — the SDK stage has not run; use
  `python3 scripts/pipeline.py` (it will build stages 1-2 as needed) or `--force=sdk`.
- **`docker buildx builder not initialized`** (sysroot stage) — run
  `docker buildx create --use --name qtbuilder`, then retry.
- **Stale `rasp.tar.gz`** after editing `Dockerfile.sysroot` — handled
  automatically; force with `--force=sysroot` if you changed only base OS packages.
- **OpenCV binaries missing** — expected unless built with `BUILD_OPENCV=ON`;
  `build-app.sh` skips the optional `opencv-binaries.tar.gz` extraction.
- **Cannot reach the Pi over SSH** — verify power/network, `PI_HOST`/`PI_PORT`, and
  that your SSH key is authorized (the probe uses `BatchMode=yes`, so password auth
  will fail fast by design).
- **Service failed to start** — inspect it with
  `ssh "$PI_USER@$PI_HOST" sudo systemctl status homenexus.service` and view logs
  with `ssh "$PI_USER@$PI_HOST" sudo journalctl -u homenexus.service -n 100`.

## Validation

After changing `scripts/pipeline.py`, run `python3 -m py_compile scripts/pipeline.py`
and the available Python lint/type checks. Use `--dry-run` to confirm the stage
plan before executing a real build.
