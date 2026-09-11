---
description: "Use when writing, modifying, reviewing, or validating HomeNexus Qt 6.11 Qt Quick/QML UI code. Covers declarative bindings, component APIs, C++ integration, accessibility, performance, and qmllint."
applyTo: "app/HomeNexus/**/*.qml"
---
# QML instructions

## Declarative structure

- Express UI state with bindings, layouts, anchors, states, and model-driven delegates. Use imperative JavaScript for commands and event handling, not to mirror values that can remain bound.
- Do not assign imperatively to a bound property unless intentionally replacing its binding. Use an intermediate property, state, or `Binding` when the relationship must later be restored or conditionally enabled.
- Use `Layout.*` attached properties for children of Qt Quick Layouts and anchors for items outside layouts. Do not anchor a layout-managed child on an axis controlled by its layout.
- Keep imports minimal and consistent with nearby files. Prefer `QtQuick.Controls.Basic` for reusable controls whose visuals are fully customized; use the existing `HomeNexus` module and local imports rather than creating parallel modules.
- Use `pragma ComponentBehavior: Bound` in files with delegates or nested components unless dynamic scoping is intentional. Pass required values explicitly and qualify outer IDs so QML tooling can analyze the component boundary.
- Order each component as imports, root object and `id`, public properties/signals, visual children, then handlers and lifecycle logic. Use `root` for reusable component and page roots, or a semantic ID such as `window` when it improves clarity; qualify access through an `id` when it avoids ambiguous or unqualified lookup.

## Properties, signals, and naming

- Name QML files and component types in PascalCase. Use lower camel case for `id` values, properties, signals, functions, and signal handlers. Retain the `App` prefix for generic reusable application controls.
- Give every public property an explicit type. Prefer `readonly property` for derived or externally immutable values, `required property` for mandatory inputs, and `property alias` only when deliberately exposing a child API. Use `var` only when the value is genuinely heterogeneous or has no useful QML type.
- Keep component APIs narrow and declarative: inputs are properties, user intent is emitted through typed signals, and internal visual items remain private. Name signals for intent, such as `backRequested`, rather than implementation details.
- When a signal handler consumes parameters, declare them explicitly with function syntax, such as `onDataChanged: function(first, last) { ... }`; do not rely on implicit parameter injection.
- Avoid duplicated state. Derive enabled, visible, text, color, and geometry values from the owning state or model, and guard event handlers when repeated notifications would trigger duplicate work.
- Use `qsTr()` for user-visible text. Do not translate identifiers, resource paths, units supplied by data models, or diagnostic output.

## Components and styling

- Reuse the controls under `ui/components` before adding page-local variants. Extract a component when behavior or styling repeats, but keep one-off page composition in its owning page.
- Use the `Style` singleton for colors, typography, spacing, radii, sizes, and `qrc` asset paths. Do not duplicate theme constants or hard-code a second visual system in pages.
- Derive reusable controls' `implicitWidth` and `implicitHeight` from their content, padding, and background when they should adapt; use fixed `Style` sizes only when the touchscreen design intentionally requires them. Let pages position controls with layouts, and keep component internals bound to the root API rather than reaching into parent or sibling objects.
- Keep delegates lightweight and self-contained. Declare required delegate/model inputs where practical, avoid dependence on injected context properties, and use `ListView` or another model view instead of manually creating repeated items.

## C++ interaction

- Keep domain, networking, persistence, and service state in the existing C++ backend. QML composes views, formats presentation values, forwards user intent, and invokes only the narrow commands exposed by a view model or registered service.
- Preserve established integrations such as the application-owned `weatherViewModel` context property and `HomeNexus` singleton services such as `NetworkStatus` and `AppNotificationCenter`. For new dependencies, prefer typed registered types or singletons for application-wide services and explicit `required property` inputs for components; do not add context properties without a project-specific reason.
- Treat C++ read-only properties as binding sources. Call invokable commands from explicit user actions or justified lifecycle handlers, and use `Connections` when reacting to C++ signals outside the exposing object's inline scope.
- When adding or moving a QML type, JavaScript helper, singleton, or asset, update `qt_add_qml_module` in `app/HomeNexus/CMakeLists.txt`; keep C++ registration and ownership changes in the C++ layer.

## Accessibility and input

- Prefer Qt Quick Controls for interactive elements so keyboard, focus, enabled, and accessibility behavior is retained. Add `Accessible.name`, `Accessible.role`, and state properties when a custom visual or icon-only control does not expose a useful semantic name.
- Make every action operable by touch and keyboard. Preserve visible focus feedback, logical tab order, and sufficiently large hit targets using the existing `Style` sizing tokens; do not make a disabled control clickable through a nested handler.
- Do not rely on color alone to communicate state. Pair warning, selection, connectivity, and validation colors with text, an icon, or another semantic cue.

## Performance and validation

- Keep frequently evaluated bindings and delegate handlers cheap. Avoid binding loops, repeated object creation in bindings, unnecessary polling timers, and synchronous work in `Component.onCompleted`, visibility handlers, or animations.
- Load expensive off-screen content only when needed, and avoid retaining hidden duplicate pages or models. Use asynchronous image loading when appropriate for non-`qrc` content and preserve stable item dimensions while content loads.
- Keep every QML file clean under the Qt 6.11 language server configured by `app/HomeNexus/.qmlls.ini`; resolve new unknown-type, unqualified-access, binding-loop, type, import, and deprecation diagnostics in touched code.
- Build the generated `appHomeNexus_qmllint` target with the configured CMake tooling after QML changes. Also build `appHomeNexus` when changing module registration, resources, C++-exposed types, or integration across the QML/C++ boundary.