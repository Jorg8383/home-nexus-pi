---
description: "Use when writing, modifying, reviewing, or validating HomeNexus Python 3 code, including build/tooling helpers under tools/ and any new Python modules, packages, tests, or CLIs. Covers project layout, typing, structure, error handling, logging, testing, dependencies, security, and validation."
applyTo: "**/*.py"
---
# Python instructions

## Language baseline

- Target Python 3 (3.9+). Write `#!/usr/bin/env python3` on executable scripts and never rely on Python 2 syntax or a bare `python` shebang.
- Follow PEP 8 layout and PEP 257 docstrings: 4-space indentation, `snake_case` for functions and variables, `PascalCase` for classes, `UPPER_SNAKE_CASE` for constants, and descriptive names over abbreviations.
- Prefer the standard library first (`pathlib`, `argparse`, `dataclasses`, `enum`, `typing`, `subprocess`, `logging`, `json`); add a third-party dependency only when it earns its place, since build/tooling scripts run where no virtualenv is guaranteed.
- Use modern idioms: f-strings over `%`/`.format()`, context managers (`with`) for files and resources, comprehensions over manual accumulation, and `pathlib.Path` over `os.path` string handling in new code.

## Structure and typing

- Guard executable entry points with `if __name__ == "__main__":` calling a `main()` function; keep import-time side effects out of modules so they stay importable and testable.
- Add type hints to public functions, methods, and module-level constants, and keep them accurate; prefer precise types (`Path`, `Sequence[str]`, `Mapping[str, int]`) over bare `list`/`dict` or `Any`.
- Keep functions small and single-purpose; group related behavior into modules or packages and expose a clear, minimal public surface. Use `dataclasses` or `enum` instead of ad-hoc tuples/dicts for structured data.
- Write concise docstrings that explain intent and non-obvious behavior; use inline comments only for what the code cannot show on its own, and delete dead or commented-out code rather than committing it.

## Inputs and configuration

- Parse command-line inputs with `argparse`, giving each argument a name, type, and help text; validate inputs and exit non-zero with a clear message when they are missing or wrong.
- Resolve and normalize incoming paths with `pathlib.Path.resolve()` before use, and operate on the resolved paths so behavior does not depend on the caller's working directory.
- Expose tunable values through arguments or environment variables with sensible defaults instead of hardcoding machine-specific paths. Never embed credentials, API keys, host names, or the contents of local config such as `homenexus.ini`.

## Error handling and logging

- Catch specific exceptions, not bare `except:` or blanket `except Exception`; let unexpected errors surface with a traceback rather than masking them, and add context when re-raising (`raise ... from err`).
- Report user-facing errors to `stderr` and exit non-zero on failure; reserve exit code `0` for success so callers in the build pipeline can branch on it.
- Use the `logging` module for diagnostics in libraries and longer-running tools instead of scattered `print` calls; keep `print` for intentional CLI output. Never log secrets or full config contents.
- Treat filesystem mutations (unlinking, re-creating symlinks, copying, removing trees) as destructive: verify preconditions first (`Path.is_symlink()`, `Path.exists()`, `Path.is_file()`) and skip entries that do not match the expected shape rather than forcing the operation.

## Security

- Never build shell commands from untrusted input; prefer `subprocess.run([...], check=True)` with an argument list and avoid `shell=True`. Do not use `eval`, `exec`, or `pickle` on untrusted data.
- Validate and sanitize external input (files, environment, network, CLI). Build paths with `Path` joins, not string concatenation of untrusted fragments, and guard against path traversal when writing outputs.
- Keep dependencies minimal and trusted; do not introduce network calls or credential handling into build/tooling scripts.

## Testing and dependencies

- Add or update focused tests when behavior changes; prefer `pytest`-style tests with small, deterministic cases and clear names. Keep tests independent of the developer's environment.
- When a script or package needs third-party libraries, declare them explicitly (for example in `pyproject.toml` or a `requirements.txt`) rather than assuming an ambient install, and pin as the project convention requires.
- Exercise scripts against a representative input (for example a scratch sysroot copy) before relying on them in the build pipeline, and confirm they exit non-zero on invalid input.

## Validation

- After changing Python code, byte-compile it with `python3 -m py_compile <file>` and, when available, format with `black`, lint with `ruff`/`flake8`, and type-check with `mypy`/`pyright`, resolving findings rather than suppressing them.
- Keep executable scripts runnable (`chmod +x`) and referenced by their existing relative paths; do not rename or relocate a tooling script without updating every caller.
- Review the resulting diff before considering the change complete, keeping edits scoped to the requested task and avoiding unrelated refactoring.
