# Contributing to minishell

This document defines the rules, structure, and expectations for contributing to the **minishell** project. Follow these standards precisely to ensure clean collaboration, maintainability, and compliance with 42 School requirements.

---

## Table of Contents

1. [Getting Started](#getting-started)

2. [Development Workflow](#development-workflow)

3. [Commit Guidelines](#commit-guidelines)

4. [Branch Management](#branch-management)

5. [Pull Request Process](#pull-request-process)

6. [Issue Reporting](#issue-reporting)

7. [Testing Guidelines](#testing-guidelines)


---

## Getting Started

1. **Fork & Clone**

    ```bash
    git clone https://github.com/your-username/minishell.git
    cd minishell
    ```

2. **Install Git Hooks**

    ```bash
    cd .githooks
    ./install.sh
    ```

    This installs a `commit-msg` hook into `.git/hooks` to enforce our commit conventions.

3. **Build**

    ```bash
    make
    ```

    Ensure a compatible C compiler and `make` are available.

4. **Run**

    ```bash
    ./minishell
    ```


---

## Development Workflow

1. **Sync with Upstream**

    ```bash
    git fetch upstream
    git checkout main
    git merge upstream/main
    ```

2. **Create a Branch**
    Follow the [Branch Management](#branch-management) rules.

3. **Implement & Test**

    - Code in small, isolated commits

    - Write or update tests for any code changes

4. **Commit Changes**
    Follow [Commit Guidelines](#commit-guidelines) strictly.

5. **Push**

    ```bash
    git push origin feature/<short-description>
    ```

6. **Then, open a Pull Request against `main`.**
  Check [Pull Request Process](#pull-request-process)


---

## Commit Guidelines

Our commits guidelines are inspired by the [Angular Commit Message Conventions](https://github.com/angular/angular/blob/main/contributing-docs/commit-message-guidelines.md).

```
<type>(<scope>): <short description>
<BLANK LINE>
[optional body]
[optional footer]
```

### Commit Types

|Type|Description|
|---|---|
|feat|A new feature|
|fix|A bug fix|
|docs|Documentation only changes|
|style|Code formatting (no logic)|
|refactor|Code restructuring (no logic)|
|test|Adding/updating tests|
|chore|Maintenance tasks|
|ci|CI/CD pipeline changes|
|build|Build system or dependencies|
|revert|Revert previous commit|

### Scope

Use the relevant module or subcomponent as the scope:

```
feat(parser): support wildcard expansion
fix(executor): handle exit status correctly
```

### Description

- Use **imperative** mood, e.g., “add”, “fix”, “remove”

- Keep line length ≤ 50 characters for the subject line

- No trailing punctuation

- Body should wrap at 72 characters

- Use **`!`**  after `<type>(<scope>)` for breaking changes


### Examples

```bash
git commit -m "feat(expansion): add tilde expansion support"
git commit -m "fix(builtin): correct cd error message"

# With breaking change
git commit -m "refactor(parser)!: change node return structure

BREAKING CHANGE: parser now returns a new data format incompatible with previous executor logic"
```

The `.githooks/install.sh` script ensures these conventions are validated automatically.

---

## Branch Management

Use the following naming pattern:

```
<type>/<short-description>
```

|Prefix|Use case|Example|
|---|---|---|
|feature/|New features|`feature/pipeline-redirection`|
|bugfix/|General bug fixes|`bugfix/missing-env-var`|
|hotfix/|Critical production fixes|`hotfix/null-pointer-crash`|
|docs/|Documentation updates|`docs/install-instructions`|
|refactor/|Code restructuring|`refactor/parser-lexer-split`|
|test/|Tests only|`test/lexer-error-cases`|

Branch off from `main`. Keep branches focused and short-lived. Merge via Pull Request.

---

## Pull Request Process

1. **Open PR against `main`**

2. **PR Description**:

    - Clearly state what changed and why

    - Reference related issues if applicable

3. **Assign Reviewers**: At least one other contributor

4. **Certify all CI pipelines and tests pass**


---

## Issue Reporting

When opening an issue, use the relevant template:

- **Bug Report**:

    - Expected vs actual behavior

    - Steps to reproduce

    - Environment info

- **Feature Request**:

    - Problem context

    - Proposed solution or feature

    - Potential impact


Always check for duplicates before creating a new issue.

---

## Testing Guidelines

### Test Framework

We use [Criterion](https://github.com/Snaipe/Criterion) for unit testing.

### File Organization

For each source file in `src/`, there must be a corresponding test file in `tests/`, named `<file>_test.c`.

**Rules:**

- **New file in `src/`** → Create corresponding `<file>_test.c`

- **New function in `src/` file** → Add new test in corresponding test file

- **Function removed** → Remove its corresponding test


**Directory Example:**

```
src/
├── parser.c
├── lexer.c
└── utils.c

tests/
├── test_parser.c
├── test_lexer.c
└── test_utils.c
```

### Writing Tests

Use Criterion’s test suites to group tests by function within each test file:

```c
TestSuite(parser);

Test(parser, should_handle_empty_input) {
    // ...
}

Test(parser, should_parse_simple_command) {
    // ...
}
```

### Running Tests

To run the full test suite:

```bash
cd test
make test
```

### Pro Tips

- Always run tests with memory analysis tools like `valgrind` during development.

- Write tests for failure cases and edge cases (e.g., null input, malformed syntax).

- Avoid testing implementation details; test _behavior_.
