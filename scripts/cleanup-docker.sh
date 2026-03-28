#!/usr/bin/env bash
set -euo pipefail

REMOVE_APP_IMAGE="${REMOVE_APP_IMAGE:-OFF}"
APP_IMAGE_NAME="${APP_IMAGE_NAME:-qt-app-image:latest}"
PRUNE_BUILD_CACHE="${PRUNE_BUILD_CACHE:-ON}"
PRUNE_DOCKER_SYSTEM="${PRUNE_DOCKER_SYSTEM:-OFF}"

if ! command -v docker >/dev/null 2>&1; then
    echo "Error: docker is not installed or not in PATH"
    exit 1
fi

echo "Docker disk usage before cleanup:"
docker system df
echo

if [[ "${REMOVE_APP_IMAGE}" == "ON" ]]; then
    echo "==> Removing app image: ${APP_IMAGE_NAME}"
    docker rmi "${APP_IMAGE_NAME}" >/dev/null 2>&1 || true
else
    echo "==> Skipping app image removal"
fi

if [[ "${PRUNE_BUILD_CACHE}" == "ON" ]]; then
    echo "==> Pruning Docker build cache"
    docker builder prune -a -f >/dev/null
else
    echo "==> Skipping Docker build cache prune"
fi

if [[ "${PRUNE_DOCKER_SYSTEM}" == "ON" ]]; then
    echo "==> Pruning Docker system (images, containers, and networks)"
    docker system prune -a -f >/dev/null
else
    echo "==> Skipping Docker system prune"
fi

echo
echo "Docker disk usage after cleanup:"
docker system df

