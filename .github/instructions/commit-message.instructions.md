---
name: "Commit Message Convention"
description: "Use when generating Git commit messages for HomeNexus. Defines Conventional Commits types, scopes, formatting, and accuracy rules."
---
# Commit message instructions

Generate commit messages that follow Conventional Commits 1.0.0:

```text
<type>[optional scope][!]: <description>

[optional body]

[optional footer(s)]
```

## Types

- Use `feat` only for a new capability in the HomeNexus application that is visible to or usable by its end users.
- Use `fix` for a bug fix.
- Use `docs` for documentation-only changes.
- Use `style` for formatting or whitespace changes that do not affect behavior.
- Use `refactor` for an internal restructuring that neither fixes a bug nor adds a feature.
- Use `perf` for a performance improvement.
- Use `test` for test-only changes.
- Use `build` for build system, dependency, or packaging changes.
- Use `ci` for continuous integration changes.
- Use `chore` for repository maintenance, editor settings, developer tooling configuration, AI instructions, and workflow guidance that do not change HomeNexus application behavior.
- Use `revert` when reverting an earlier commit.

Choose the most specific type supported by the staged changes.
Do not use `feat` merely because a change adds a new file, setting, automation, developer capability, or repository configuration.
When the staged changes only configure VS Code, Copilot, commit generation, or instruction files, use `chore`.

## Scopes

- Add a lowercase scope when one area clearly owns the change.
- Prefer project scopes such as `weather`, `ui`, `appservice`, `config`, `build`, `deploy`, or `docs`.
- Use a concise component or subsystem name when none of the preferred scopes fit.
- Omit the scope for genuinely cross-cutting changes.

## Subject

- Write the description in imperative mood and lowercase its first word.
- Describe the outcome, not the editing activity.
- Do not end the description with a period.
- Keep the complete subject line at or below 72 characters.

## Body and footers

- Add a body only when the motivation, behavior, or important implementation context is not clear from the subject.
- Separate the body from the subject with one blank line.
- Explain why the change was needed and any important behavioral consequences; do not restate the subject.
- Add issue or review footers only when their identifiers are present in the staged changes or supplied context.
- Mark an incompatible change with `!` before the colon and describe it in a `BREAKING CHANGE: <description>` footer.
- Never infer or invent a breaking change, issue identifier, test result, or external reference.

## Accuracy

- Describe only staged changes.
- Do not claim behavior, validation, or compatibility that the staged diff does not demonstrate.
- Return only the commit message, without Markdown fences, alternatives, or commentary.

## Examples

```text
feat(weather): add hourly forecast model
```

```text
fix(ui): preserve weather page bindings after refresh
```

```text
docs: document Raspberry Pi deployment overrides
```

```text
chore(config): add Conventional Commit generation guidance
```

```text
feat(config)!: require explicit weather provider

BREAKING CHANGE: deployments must now configure a weather provider.
```