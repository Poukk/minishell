# minishell

A simple UNIX shell implementation in C, built as part of the 42 School curriculum.

## Overview

Minishell is a basic shell that replicates core functionality of bash, including command execution, piping, redirections, and built-in commands.

## Features

- Execute commands with arguments
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Input/output redirections (`<`, `>`, `>>`)
- Pipes (`|`)
- Environment variable expansion (`$VAR`)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)
- Command history

## Requirements

- GCC compiler
- GNU Make
- Readline library

## Installation

```bash
git clone <repository-url>
cd minishell
make
```

## Usage

```bash
./minishell
```

Start typing commands like you would in any shell:

```bash
minishell$ ls -la
minishell$ echo "Hello World"
minishell$ cat file.txt | grep "pattern"
minishell$ export PATH=/usr/bin
minishell$ exit
```

## Project Structure

- `src/` - Source code files
- `include/` - Header files
- `libft/` - Custom C library
- `Makefile` - Build configuration

## Testing

Tests are implemented using the Criterion framework:

```bash
make test
```

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines.

## License

This project is part of the 42 School curriculum.