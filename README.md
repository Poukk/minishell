# Minishell

A simplified shell implementation written in C, inspired by bash. This project is part of the 42 curriculum and demonstrates core shell functionality including command parsing, execution, piping, redirections, and built-in commands.

## Features

### Core Shell Functionality
- **Interactive prompt** with command history using GNU readline
- **Command parsing** with Abstract Syntax Tree (AST) construction
- **Command execution** with proper process management
- **Environment variable** management and expansion
- **Signal handling** for SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\)

### Supported Features
- **Pipes** (`|`) - Connect multiple commands
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append mode (`>>`)
  - Heredocs (`<<`)
- **Quotes handling**:
  - Single quotes (`'`) - literal interpretation
  - Double quotes (`"`) - allow variable expansion
- **Variable expansion** (`$VAR` and `$?` for exit status)

### Built-in Commands
| Command | Description |
|---------|-------------|
| `echo` | Display text with `-n` option support |
| `cd` | Change current directory |
| `pwd` | Print working directory |
| `env` | Display environment variables |
| `export` | Set/export environment variables |
| `unset` | Remove environment variables |
| `exit` | Exit the shell with optional exit code |

## Project Structure

```
.
├── include/          # Header files
├── src/
│   ├── builtins/     # Built-in command implementations
│   ├── core/         # Main shell logic and memory management
│   ├── environment/  # Environment variable handling
│   ├── error/        # Error handling
│   ├── executor/     # Command execution logic
│   ├── expansion/    # Variable expansion and heredoc processing
│   ├── io/           # I/O redirection and heredoc handling
│   ├── parser/       # Command parser and AST builder
│   ├── signals/      # Signal handling
│   ├── tokenizer/    # Lexical analysis
│   └── utils/        # Utility functions
├── lib/              # External libraries (libft)
└── Makefile
```

## Building

### Requirements
- GCC or Clang compiler
- GNU Readline library
- Make

### Compilation
```bash
make
make clean
make fclean
make re
```

## Usage

```bash
# Run the shell
./minishell

# Interactive mode
minishell$ ls -la | grep minishell
minishell$ echo "Hello, World!"
minishell$ export MY_VAR=value
minishell$ echo $MY_VAR
minishell$ exit
```

### Example Commands
```bash
# Simple commands
minishell$ ls
minishell$ pwd

# Pipes
minishell$ ls -la | grep txt | wc -l

# Redirections
minishell$ echo "hello" > file.txt
minishell$ cat < file.txt
minishell$ echo "world" >> file.txt

# Heredocs
minishell$ cat << EOF
> This is a heredoc
> Multiple lines supported
> EOF

# Environment variables
minishell$ echo $HOME
minishell$ echo $?
minishell$ export PATH=$PATH:/new/path
```

## Architecture

### Execution Flow
1. **Readline** - Read user input with history support
2. **Tokenizer** - Break input into tokens (words, operators)
3. **Parser** - Build Abstract Syntax Tree (AST) from tokens
4. **Expansion** - Expand variables and process heredocs
5. **Executor** - Execute commands, handle pipes and redirections

### Memory Management
The shell uses a garbage collector (`t_gc`) to manage memory during command execution, ensuring proper cleanup after each command.

### Signal Handling
- `Ctrl+C` (SIGINT): Interrupt current command, display new prompt
- `Ctrl+\` (SIGQUIT): Ignored in interactive mode
- `Ctrl+D` (EOF): Exit the shell

## Known Limitations

This is an educational implementation with some limitations compared to full bash:
- No support for `&&`, `||`, or `;` operators
- No wildcards/globbing (`*`, `?`)
- No command substitution (`$(command)` or `` `command` ``)
- Limited history expansion features

## License

This project is part of the 42 School curriculum. See the 42 School guidelines for usage terms.
