#!/usr/bin/env bash
set -euo pipefail

# ------------------------------------------------------------
# Configuration
# ------------------------------------------------------------

PI_USER="${PI_USER:-strjo2}"
PI_HOST="${PI_HOST:-192.168.178.36}"
PI_PORT="${PI_PORT:-22}"

SSH_TARGET="${PI_USER}@${PI_HOST}"
SSH_OPTS=(-p "${PI_PORT}")
SCP_OPTS=(-P "${PI_PORT}")

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
ARTIFACTS_DIR="${ROOT_DIR}/artifacts"

QT_TARBALL="${QT_TARBALL:-${ARTIFACTS_DIR}/qt-pi-binaries.tar.gz}"

# Change the app name as required
APP_LOCAL_PATH="${APP_LOCAL_PATH:-${ARTIFACTS_DIR}/appHomeNexus}"

# Local config file. This should not be committed to Git.
APP_CONFIG_LOCAL_PATH="${APP_CONFIG_LOCAL_PATH:-${ROOT_DIR}/app/HomeNexus/config/homenexus.ini}"

QT_INSTALL_DIR="${QT_INSTALL_DIR:-/usr/local/qt6}"
APP_REMOTE_DIR="${APP_REMOTE_DIR:-/home/${PI_USER}/app}"
APP_CONFIG_REMOTE_DIR="${APP_CONFIG_REMOTE_DIR:-/etc/homenexus}"
APP_CONFIG_REMOTE_FILE="${APP_CONFIG_REMOTE_FILE:-${APP_CONFIG_REMOTE_DIR}/homenexus.ini}"

REMOTE_SETUP_SCRIPT="${REMOTE_SETUP_SCRIPT:-${SCRIPT_DIR}/remote-setup.sh}"
REMOTE_SETUP_PATH="/home/${PI_USER}/remote-setup.sh"

# ------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------

die() {
    echo "Error: $*" >&2
    exit 1
}

require_file() {
    local file="$1"
    [[ -f "${file}" ]] || die "Required file not found: ${file}"
}

require_command() {
    local cmd="$1"
    command -v "$cmd" >/dev/null 2>&1 || die "Required command not found in PATH: ${cmd}"
}

# ------------------------------------------------------------
# Preflight checks
# ------------------------------------------------------------

require_command scp
require_command ssh
require_file "${QT_TARBALL}"
require_file "${APP_LOCAL_PATH}"
require_file "${REMOTE_SETUP_SCRIPT}"

# The config is optional. The app can start without it and use defaults.
if [[ -f "${APP_CONFIG_LOCAL_PATH}" ]]; then
    DEPLOY_CONFIG="1"
else
    DEPLOY_CONFIG="0"
    echo "Warning: Config file not found: ${APP_CONFIG_LOCAL_PATH}"
    echo "         The app will be deployed without homenexus.ini."
fi

# ------------------------------------------------------------
# Copy files to the Raspberry Pi
# ------------------------------------------------------------

echo "==> Copying Qt tarball to target"
scp "${SCP_OPTS[@]}" "${QT_TARBALL}" "${SSH_TARGET}:/home/${PI_USER}/"

echo "==> Copying app binary to target"
scp "${SCP_OPTS[@]}" "${APP_LOCAL_PATH}" "${SSH_TARGET}:/home/${PI_USER}/"

if [[ "${DEPLOY_CONFIG}" == "1" ]]; then
    echo "==> Copying config file to target"
    scp "${SCP_OPTS[@]}" "${APP_CONFIG_LOCAL_PATH}" "${SSH_TARGET}:/home/${PI_USER}/homenexus.ini"
fi

echo "==> Copying remote setup script to target"
scp "${SCP_OPTS[@]}" "${REMOTE_SETUP_SCRIPT}" "${SSH_TARGET}:${REMOTE_SETUP_PATH}"

# ------------------------------------------------------------
# Remote setup
# ------------------------------------------------------------

echo "==> Executing remote setup"
ssh "${SSH_OPTS[@]}" "${SSH_TARGET}" \
    bash "${REMOTE_SETUP_PATH}" \
    "/home/${PI_USER}/$(basename "${QT_TARBALL}")" \
    "${QT_INSTALL_DIR}" \
    "${APP_REMOTE_DIR}" \
    "$(basename "${APP_LOCAL_PATH}")" \
    "${APP_CONFIG_REMOTE_FILE}" \
    "${DEPLOY_CONFIG}"
echo "==> Done"