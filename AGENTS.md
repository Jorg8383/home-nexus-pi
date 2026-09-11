# HomeNexus workspace instructions

## Repository scope

- HomeNexus is a Raspberry Pi smart-home dashboard for a 7-inch touchscreen.
- The application lives under `app/HomeNexus` and is built as the `appHomeNexus` executable.
- `main.cpp` is the application entry point and `Main.qml` is the QML root.
- The QML module and UI are under `app/HomeNexus/ui`; reusable UI components, pages, theme, data, and utilities are kept there.
- Backend code is split into static libraries under `app/HomeNexus/src`: weather integration and models, application services, and JSON utilities. QML integration wrappers are under `app/HomeNexus/src/qml`.
- Unit tests currently live under `app/HomeNexus/tests/weather` and cover the weather client and parser.
- For the broader product scope and current status, see [README.md](README.md). Architecture diagrams are in `docs/`.

## Technology and architecture

- The application uses Qt 6.11, Qt Quick/QML, C++17, CMake, and CTest. The configured host generator is Ninja.
- `app/HomeNexus` owns the executable and QML module. `main.cpp` starts the application; `Main.qml` is the QML entry point.
- `app/HomeNexus/ui` contains the touchscreen UI, including reusable components, pages, theme, data models, and utilities.
- `app/HomeNexus/src/weather` contains weather API clients, parsing, models, fallback handling, and weather view-model logic.
- `app/HomeNexus/src/appservice` contains application configuration, notifications, and network status.
- `app/HomeNexus/src/qml` contains foreign-type wrappers that expose selected application services to QML.
- `app/HomeNexus/src/utils` contains shared JSON utilities.
- The backend directories are built as static libraries and linked into `appHomeNexus`; keep behavior in the owning library and expose only the interfaces needed by the QML layer.
- The current automated tests are Qt Test executables under `app/HomeNexus/tests/weather`, registered with CTest.

## Build and test

- The CMake source directory is `app/HomeNexus`. It requires Qt 6.11 and C++17; CTest is enabled by the project.
- The configured host build directories are `app/HomeNexus/builds/Qt-6.11.1-gcc_64-x86_64/Debug` and `Release`.
- Build the application with:

  ```bash
  cmake --build app/HomeNexus/builds/Qt-6.11.1-gcc_64-x86_64/Debug --target appHomeNexus
  ```

  Use the `Release` directory when that is the selected configuration.
- Run the configured tests with:

  ```bash
  ctest --test-dir app/HomeNexus/builds/Qt-6.11.1-gcc_64-x86_64/Debug
  ```

- Do not assume a build directory exists or is configured. The VS Code CMake configuration in `.vscode/settings.json` documents the local Qt prefix and build settings.

## Raspberry Pi build workflow

- The Docker cross-build is a three-stage pipeline and must run in order:
  1. `bash scripts/build-sysroot.sh` creates `artifacts/rasp.tar.gz`.
  2. `bash scripts/build-sdk-image.sh` consumes that tarball and creates the `qtcrossbuild:latest` image.
  3. `bash scripts/build-app.sh` consumes the SDK image and extracts `artifacts/appHomeNexus`, `artifacts/qt-pi-binaries.tar.gz`, and the optional `artifacts/opencv-binaries.tar.gz`.
- Run all three stages with `bash scripts/build-all.sh`. `BUILD_OPENCV=ON` is an optional input to the SDK stage.
- Keep changes to the Dockerfiles and shell scripts aligned with the preflight checks and environment variables already defined in those files.

## Configuration and runtime data

- `app/HomeNexus/config/homenexus.example.ini` is the committed configuration template. `homenexus.ini` is local/runtime configuration and must not be committed.
- The CMake post-build step copies the configuration template to the application output as `config/homenexus.ini`. It also copies the two committed fallback weather files from `app/HomeNexus/data/weather`.
- Preserve the existing relative layout for `config/` and `data/weather/`; the application and deployment scripts rely on it.

## Deployment

- `bash scripts/deploy.sh` deploys the extracted binary, Qt runtime archive, optional local config, and fallback data over SSH.
- The deployment script contains machine-specific default host and user values. Review or override `PI_USER`, `PI_HOST`, `PI_PORT`, and related paths before running it; do not treat those defaults as portable environment assumptions.
- Do not deploy or expose credentials, API keys, or local configuration contents in source changes.

## Change guidance

- Prefer existing CMake targets, library boundaries, scripts, configuration mechanisms, and data paths over introducing parallel approaches.
- Keep UI-focused changes in the QML layer and backend behavior in the C++ backend, following existing project patterns.
- Update or add focused tests when behavior changes.
- Keep changes scoped to the requested task. Avoid unrelated refactoring unless it is necessary for correctness or explicitly requested.
- Do not modify or commit generated build output, cross-build artifacts, caches, local configuration, or other machine-generated files.
- Respect repository configuration such as `.clang-format`, `.clang-tidy`, and `app/HomeNexus/.qmlls.ini`. The `.qmlls.ini` paths are machine-specific; do not treat them as portable defaults.
- Before considering a task complete, review the resulting diff and run the relevant build, tests, and static or lint checks available for the files changed.