#!/usr/bin/env bash
set -euo pipefail

IMAGE_NAME="${IMAGE_NAME:-qtcrossbuild:latest}"
BUILD_OPENCV="${BUILD_OPENCV:-OFF}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DOCKER_DIR="${ROOT_DIR}/docker"
ARTIFACTS_DIR="${ROOT_DIR}/artifacts"
SYSROOT_TARBALL="${ARTIFACTS_DIR}/rasp.tar.gz"

# ------------------------------------------------------------
# Preflight checks
# ------------------------------------------------------------

if ! command -v docker >/dev/null 2>&1; then
    echo "Error: docker is not installed or not in PATH"
    exit 1
fi

if [[ ! -f "${DOCKER_DIR}/Dockerfile.sdk" ]]; then
    echo "Error: ${DOCKER_DIR}/Dockerfile.sdk does not exist."
    exit 1
fi

if [[ ! -f "${SYSROOT_TARBALL}" ]]; then
    echo "Error: ${SYSROOT_TARBALL} does not exist."
    echo "Run build-sysroot.sh first."
    exit 1
fi

if [[ "${BUILD_OPENCV}" != "ON" && "${BUILD_OPENCV}" != "OFF" ]]; then
    echo "Error: BUILD_OPENCV must be ON or OFF"
    exit 1
fi

# ------------------------------------------------------------
# Build Qt cross-compile SDK image
# ------------------------------------------------------------

echo "==> Building SDK image: ${IMAGE_NAME}"
echo "==> BUILD_OPENCV=${BUILD_OPENCV}"

docker build \
    -f "${DOCKER_DIR}/Dockerfile.sdk" \
  -t "${IMAGE_NAME}" \
  --build-arg BUILD_OPENCV="${BUILD_OPENCV}" \
  "${ROOT_DIR}"

echo "==> SDK image build complete"