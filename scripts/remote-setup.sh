#!/usr/bin/env bash
set -euo pipefail

# ------------------------------------------------------------
# Arguments
# ------------------------------------------------------------

QT_TARBALL="${1:?Missing Qt tarball path}"
QT_INSTALL_DIR="${2:?Missing Qt install directory}"
APP_REMOTE_DIR="${3:?Missing app remote directory}"
APP_BINARY_NAME="${4:?Missing app binary name}"
APP_CONFIG_FILE="${5:?Missing app config file path}"
DEPLOY_CONFIG="${6:-0}"

APP_BINARY_SRC="$(dirname "${QT_TARBALL}")/${APP_BINARY_NAME}"
APP_CONFIG_SRC="$(dirname "${QT_TARBALL}")/homenexus.ini"

# ------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------

die() {
    echo "Error: $*" >&2
    exit 1
}

# ------------------------------------------------------------
# Preflight checks
# ------------------------------------------------------------

[[ -f "${QT_TARBALL}" ]] || die "Qt tarball not found: ${QT_TARBALL}"
[[ -f "${APP_BINARY_SRC}" ]] || die "App binary not found: ${APP_BINARY_SRC}"

if [[ "${DEPLOY_CONFIG}" == "1" ]]; then
    [[ -f "${APP_CONFIG_SRC}" ]] || die "Config file not found: ${APP_CONFIG_SRC}"
fi

# ------------------------------------------------------------
# Remote setup
# ------------------------------------------------------------

echo "==> Creating Qt install directory"
sudo mkdir -p "${QT_INSTALL_DIR}"

echo "==> Extracting Qt runtime"
sudo tar -xzf "${QT_TARBALL}" -C "${QT_INSTALL_DIR}"

echo "==> Creating app directory"
mkdir -p "${APP_REMOTE_DIR}"

echo "==> Installing app binary"
cp -f "${APP_BINARY_SRC}" "${APP_REMOTE_DIR}/"
chmod +x "${APP_REMOTE_DIR}/${APP_BINARY_NAME}"

if [[ "${DEPLOY_CONFIG}" == "1" ]]; then
    echo "==> Installing app config"
    sudo mkdir -p "$(dirname "${APP_CONFIG_FILE}")"
    sudo cp -f "${APP_CONFIG_SRC}" "${APP_CONFIG_FILE}"

    # Make config readable by the app user only.
    sudo chown "${USER}:${USER}" "${APP_CONFIG_FILE}"
    chmod 600 "${APP_CONFIG_FILE}"
else
    echo "==> No app config deployed"
    echo "    The application will use defaults and disable online weather if no API key is available."
fi

echo "==> Registering Qt libraries with dynamic linker"
echo "${QT_INSTALL_DIR}/lib" | sudo tee /etc/ld.so.conf.d/qt6.conf >/dev/null
sudo ldconfig

echo "==> Creating launcher script"
cat > "${APP_REMOTE_DIR}/run-app.sh" <<EOF
#!/usr/bin/env bash
set -euo pipefail
export LD_LIBRARY_PATH="${QT_INSTALL_DIR}/lib:\${LD_LIBRARY_PATH:-}"
export DISPLAY="\${DISPLAY:-:0}"
exec "${APP_REMOTE_DIR}/${APP_BINARY_NAME}" --embedded --config "${APP_CONFIG_FILE}"
EOF

chmod +x "${APP_REMOTE_DIR}/run-app.sh"

echo "==> Deployment finished"
echo "Qt installed in: ${QT_INSTALL_DIR}"
echo "App installed in: ${APP_REMOTE_DIR}"
echo "Config file path: ${APP_CONFIG_FILE}"
echo
echo "Start app with:"
echo "  ${APP_REMOTE_DIR}/run-app.sh"
