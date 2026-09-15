---
description: "Use when writing, modifying, reviewing, or validating HomeNexus Bash scripts under scripts/. Covers the cross-build and deploy pipeline, strict mode, parameterization, preflight checks, quoting, and safe command execution."
applyTo: "scripts/**/*.sh"
---
# Bash instructions

## Script baseline

- Start every script with `#!/usr/bin/env bash` and `set -euo pipefail`. Do not weaken strict mode; if a command may legitimately fail, guard it explicitly (for example `cmd || true`) rather than disabling `errexit`.
- Resolve locations from the script itself: `SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"` and derive `ROOT_DIR` from it. Do not assume the caller's working directory.
- Keep the existing section-comment structure (Configuration, Arguments, Preflight checks, Helper functions, and the ordered build/deploy stages). Preserve the numbered stage ordering in the pipeline scripts.
- Preserve the three-stage cross-build contract: `build-sysroot.sh` → `build-sdk-image.sh` → `build-app.sh`, orchestrated by `build-all.sh`. Do not merge stages or introduce a parallel build path.

## Inputs and configuration

- Expose tunables as environment variables with defaults using `${VAR:-default}` (for example `BUILD_TYPE`, `APP_DIR`, `APP_BINARY_NAME`, `BUILD_OPENCV`, `PI_USER`, `PI_HOST`). Reuse the existing variable names and defaults instead of inventing parallel ones.
- Treat required positional arguments as mandatory with `${N:?Missing ...}` and keep the documented argument order stable, since callers such as `deploy.sh` pass them positionally.
- Keep machine-specific values (`PI_USER`, `PI_HOST`, `PI_PORT`, install paths) overridable through environment variables. Do not hardcode new host-specific defaults, and never embed credentials, API keys, or the contents of local config such as `homenexus.ini`.

## Safety and robustness

- Run preflight checks before side effects: verify required files with `[[ -f ... ]]`, executables with `[[ -x ... ]]`, tools with `command -v <tool> >/dev/null 2>&1`, and Docker images with `docker image inspect`. Fail early with a clear message and non-zero exit.
- Use `[[ ]]` for conditionals and `$(...)` for command substitution. Quote all expansions (`"${VAR}"`), and use arrays for command options that must stay separate words (for example `SSH_OPTS=(-p "${PI_PORT}")`).
- Register cleanup for resources that outlive a single command with `trap cleanup EXIT`, and make cleanup idempotent (for example `docker rm -f "${CONTAINER_NAME}" >/dev/null 2>&1 || true`).
- Report errors to stderr and exit non-zero; prefer a small `die() { echo "Error: $*" >&2; exit 1; }` helper where the script already uses one. Emit progress with the existing `==>` / banner style.

## Deployment and remote execution

- Keep deployment packaging and remote actions in the existing `deploy.sh` and `remote-setup.sh` flow. Do not duplicate this logic into post-build staging or other scripts.
- Pass SSH/SCP options through the established arrays and honor `PI_PORT`. Preserve the remote directory layout (`QT_INSTALL_DIR`, `APP_REMOTE_DIR`, `APP_DATA_REMOTE_DIR`, `APP_CONFIG_REMOTE_DIR`) that the application relies on at runtime.
- Limit `sudo` to operations that genuinely require elevation (installing the Qt runtime, writing system config) and keep those steps explicit and reviewable.

## Validation

- After changing a script, check it with `bash -n <script>` and, when available, `shellcheck <script>`, and resolve findings rather than suppressing them.
- Keep scripts executable (`chmod +x`) and referenced by their existing relative paths; do not rename or relocate pipeline scripts without updating every caller.
