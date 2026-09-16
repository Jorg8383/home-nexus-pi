---
description: "Use when writing, modifying, reviewing, or testing HomeNexus C++ and Qt 6 code. Covers C++17, QObject lifetime, signals, properties, QML integration, Qt Test, formatting, and static analysis."
applyTo: "**/*.{cpp,h,hpp}"
---
# C++ and Qt instructions

## Language and structure

- Write C++17; do not introduce features that require a newer language standard.
- Follow the existing organization: PascalCase class and file names, camelCase functions, `m_`-prefixed data members, and `I`-prefixed interfaces.
- Use `#pragma once`. Keep declarations in `.hpp` files and non-trivial implementations in matching `.cpp` files.
- Prefer type-safe C++17 constructs such as `enum class`, `constexpr`, `override`, `nullptr`, scoped resource management, and const-correct APIs.
- Mark single-argument constructors `explicit` unless implicit conversion is intentional.
- Prefer value semantics and RAII for non-QObject resources; use smart pointers when dynamic ownership is required. Raw QObject pointers may refer to parent-owned or non-owning objects but must not imply undocumented ownership. Give polymorphic interfaces virtual destructors.
- Use `QStringLiteral` and `QByteArrayLiteral` for fixed Qt string and byte-array literals.
- Keep dependencies explicit. Pass required, non-owning collaborators by reference and ensure they outlive the receiving object.

## Design principles (SOLID)

- Single responsibility: keep each backend static library focused on a single concern, keep view-models thin and free of transport or persistence logic, and keep QML foreign wrappers free of service behavior.
- Open/closed: extend behavior by adding new implementations of an `I`-prefixed interface or injecting a different collaborator, not by editing stable types or growing conditional branches in existing ones.
- Liskov substitution: any implementation of an `I`-interface must honor the interface's contract, including its error and signal semantics; give polymorphic bases virtual destructors and mark overrides `override`.
- Interface segregation: expose narrow, role-specific interfaces to QML and to collaborators; prefer several focused interfaces over one broad interface that forces clients to depend on members they do not use.
- Dependency inversion: depend on `I`-prefixed abstractions passed by reference, and construct concrete types at composition points such as `main.cpp` rather than deep in domain code.

## QObject ownership and lifetime

- Give every owned `QObject` an unambiguous lifetime: use Qt parent-child ownership or stack ownership, following the nearby implementation.
- Keep parent and child QObjects in the same thread. For stack-allocated parent-child objects, construct the parent first so reverse destruction order destroys the child first.
- When a QObject-derived type supports optional parent ownership, accept `QObject *parent = nullptr` and forward it to the base class.
- Do not manually delete parent-owned QObjects. When an object must be destroyed safely after event processing, as with a completed `QNetworkReply`, use `deleteLater()` rather than deleting it inside its callback.
- Never capture references to locals in callbacks that can outlive the current scope. Use explicit lambda captures and provide a context object to `connect`; Qt then disconnects automatically when the context is destroyed.
- Keep mandatory injected dependencies as references. Use a pointer only when absence is meaningful, and document ownership when it is not evident. Use `QPointer<T>` for a nullable, externally owned QObject that may be destroyed independently.

## Signals, slots, and properties

- Use compile-time checked signal/slot connections: `&Sender::signal` and `&Receiver::slot`; do not use string-based `SIGNAL` or `SLOT` syntax.
- Use queued delivery for cross-thread calls. Register custom queued signal/slot argument types with Qt's meta-type system, and do not access a QObject directly from a different thread.
- Establish persistent connections during construction where practical. Name private handlers `on...` consistently with existing classes.
- Emit change signals only after the backing state changes; guard against redundant notifications when appropriate.
- For each `Q_PROPERTY`, provide a const-correct getter and a matching `NOTIFY` signal when the value can change. Use `CONSTANT` only for values that remain stable for the object's lifetime.
- Prefer read-only properties unless assignment from QML is an intentional part of the type's public API. Expose commands with `Q_INVOKABLE` only when QML must call them.
- Keep asynchronous failures in the existing Qt flow: emit error signals and return early. Do not allow exceptions to cross signal/slot or QML boundaries.

## C++ and QML interaction

- Keep domain and service behavior in the owning C++ library; expose a narrow view-model or service interface to QML.
- Follow the existing exposure mechanism for the type being changed: root context properties for application-owned view models and foreign wrappers under `app/HomeNexus/src/qml` for C++-owned singleton services.
- For `QML_FOREIGN` singletons, retain C++ ownership with `QQmlEngine::CppOwnership`, require a valid instance before QML creation, initialize it before loading the QML module, and keep its thread affinity compatible with the QML engine. Marshal any cross-thread calls explicitly.
- Keep QML registration wrappers free of service behavior. When adding an exposed type, update its QML module registration and static-library metatype integration consistently with neighboring types.
- For `QAbstractListModel`, use typed roles beginning at `Qt::UserRole + 1`, validate indexes and bounds, keep `roleNames()` synchronized, and wrap mutations in the correct `begin...`/`end...` calls.

## Tests

- Add focused Qt Test coverage for changed behavior. Test classes inherit `QObject`, declare test functions as private slots, and use descriptive behavior names. Include the generated `.moc` when the `Q_OBJECT` test class is declared in the `.cpp` file. Select `QTEST_MAIN`, `QTEST_GUILESS_MAIN`, or `QTEST_APPLESS_MAIN` according to the application and event-loop facilities the test requires.
- Use `QVERIFY` for conditions and `QCOMPARE` for values. Use `QSignalSpy` when signal emission or payloads are part of the contract.
- For asynchronous behavior, use `QTRY_VERIFY` or `QTRY_COMPARE` with an appropriate timeout instead of fixed sleeps or timing assumptions.
- Put reusable fixtures under the test's `testdata/` directory and locate them with `QFINDTESTDATA`; avoid machine-specific absolute paths.
- Cover failure paths and boundary conditions as well as successful behavior, especially for parsing, network responses, property notifications, and model indexes.

## Formatting and static analysis

- Format touched C++ with the repository `.clang-format`: Allman braces, four spaces, no tabs, 100-column limit, and right-aligned pointer/reference declarators.
- Preserve intentional include ordering; the formatter has `SortIncludes: Never`.
- Keep code clean under the repository `.clang-tidy` checks, currently `clang-analyzer-*` and `cppcoreguidelines-*`. Resolve new findings in changed code rather than broadly suppressing them.
- Add comments only when ownership, asynchronous lifetime, or non-obvious behavior cannot be made clear in the code itself.
