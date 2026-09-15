---
description: "Use when writing, modifying, reviewing, or validating HomeNexus CMake build files and Raspberry Pi toolchains. Covers Qt 6 targets, QML registration, dependency visibility, tests, runtime staging, and cross-compilation."
applyTo: "app/HomeNexus/**/CMakeLists.txt, cmake/**/*.cmake"
---
# CMake instructions

## Application build files

- Preserve the application baseline in `app/HomeNexus/CMakeLists.txt`: CMake 3.16 compatibility, C++17, and `qt_standard_project_setup(REQUIRES 6.11)`. Do not require newer versions without an intentional project-wide change.
- Keep package discovery, `include(CTest)`, and top-level subdirectory wiring in the application root. Put each backend library's sources and target settings in its owning `src/<module>/CMakeLists.txt`, and each test module's targets in its owning `tests/<module>/CMakeLists.txt`.
- Extend the existing `HomeNexusWeather`, `HomeNexusAppService`, and `HomeNexusUtils` static libraries when behavior belongs to those modules. Add a new library only for a distinct ownership boundary, then wire it through `src/CMakeLists.txt` and link it to the narrowest consumer.
- Prefer target commands such as `target_link_libraries`, `target_include_directories`, `target_compile_features`, and `target_compile_definitions` over directory-wide settings. Retain the root language baseline; libraries expose `cxx_std_17` as an existing usage requirement.
- Mark a dependency or include directory `PUBLIC` only when consumers need it through the target's public headers or usage requirements. Keep implementation-only Qt modules and internal libraries `PRIVATE`.
- Keep Qt package discovery in the root `find_package(Qt6 ...)`. When introducing a direct dependency on another Qt module, add its component there and link the imported `Qt6::<Module>` target only to the targets that use it.

## Qt and QML integration

- Keep `appHomeNexus` as both the executable and the `HomeNexus` QML module unless a deliberate architecture change requires otherwise.
- List QML files and JavaScript helpers explicitly under `QML_FILES`, C++ registration wrappers under `SOURCES`, and packaged assets under `RESOURCES` in `qt_add_qml_module`. Update the applicable `set_source_files_properties` call for QML singletons and helpers that must be omitted from the generated `qmldir`.
- Preserve `QT_QML_MODULE_NO_IMPORT_SCAN` and `QT_SKIP_AUTO_QML_PLUGIN_INCLUSION` unless the related static-QML import behavior is intentionally changed and validated.
- Call `qt_extract_metatypes(<target>)` for a static library whose C++ types are exposed through `QML_FOREIGN` wrappers. Keep registration wrappers in `app/HomeNexus/src/qml` and include them in the application module's `SOURCES` list.
- Keep domain and service targets independent of UI resources. Link backend libraries into `appHomeNexus`; keep backend implementation files in their owning libraries rather than moving them into the QML module merely to make registration work.

## Tests and runtime data

- Define Qt Test binaries with `qt_add_executable`, link `Qt6::Test` and the targets under test as `PRIVATE`, and register test binaries intended for the automated suite with `add_test` so they run through CTest.
- Set a test `WORKING_DIRECTORY` only when existing relative fixture lookup requires it. For new C++ test fixtures, prefer a `testdata/` directory located with `QFINDTESTDATA` over source-tree assumptions or absolute paths.
- Use target-based `POST_BUILD` commands only for build-time runtime staging. Preserve the output layout for `config/homenexus.ini` and `data/weather/*.json`; application startup and deployment depend on those relative paths.
- Keep deployment packaging in `install()` and the existing deployment scripts. Do not turn post-build staging into a second deployment mechanism.

## Raspberry Pi toolchains

- Treat `cmake/toolchain.cmake` and `cmake/opencvtoolchain.cmake` as Docker cross-build inputs, not portable host presets. Preserve their CMake 3.25 baseline, `aarch64-linux-gnu` compiler contract, sysroot paths, architecture flags, and CMake find-root modes unless the SDK image changes with them.
- When changing a compiler, sysroot, Qt prefix, library path, or architecture, update the affected inputs in `docker/Dockerfile.sdk`, `docker/Dockerfile.app`, and the `scripts/build-*.sh` pipeline in the same change. The producing container and consuming build must agree.
- Keep application target settings out of the toolchain files. Toolchain-wide compiler and linker settings are reserved for cross-compilation requirements that must be established before project configuration.
- Preserve the distinction between the base and OpenCV toolchains. Do not copy OpenCV-specific linker libraries or Cortex-A72 tuning into the base toolchain without validating both cross-build paths.

## Maintenance and validation

- Use lowercase CMake commands, four-space indentation inside command argument lists, one argument per line for non-trivial source or dependency lists, and target names consistent with neighboring manifests.
- Do not edit generated build trees, `CMakeCache.txt`, generated `compile_commands.json`, or files under `artifacts/` to implement a build-system change.
- Validate application manifest changes by reconfiguring when package discovery, source lists, target properties, or toolchain inputs change, then build the narrowest affected target. Run the relevant CTest tests when test registration or linked backend behavior changes.
- Follow `AGENTS.md` for the configured build, test, and cross-build commands; do not introduce parallel build directory conventions in CMake files.