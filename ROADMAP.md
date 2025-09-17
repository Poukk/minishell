# Minishell Roadmap

Use this checklist to track progress and identify next steps.

### Phase 1: The Core Loop, Lexer, and Scoped Allocator ✅
*   **Objective:** Read and tokenize user input while ensuring robust memory management from the start.

-   [x] **Project Setup:** Create `Makefile`, header files, and `.gitignore`.
-   [x] **Readline Loop:** Implement the main loop using `readline` to get user input.
-   [x] **Handle EOF:** Gracefully exit the shell on `Ctrl-D` (when `readline` returns `NULL`).
-   [x] **Scoped Allocator:** Implement the `gc_malloc()` and `gc_free_all()` system to manage memory for each command line.
-   [x] **Lexer (Tokenizer):** Write a function to convert the input string into a linked list of tokens.
-   [x] **Lexer - Delimiters:** Handle spaces and tabs as token separators.
-   [x] **Lexer - Metacharacters:** Identify `|`, `<`, `>` as separate tokens.
-   [x] **Lexer - Quoting:** Correctly handle single (`'`) and double (`"`) quotes, treating their content as a single token.
-   [x] **Testing:** Create a simple main to print tokens and verify no memory leaks with `valgrind` or `leaks`.

### Phase 2: The Parser and Abstract Syntax Tree (AST) ✅
*   **Objective:** Convert the flat list of tokens into a hierarchical command tree (AST).

-   [x] **AST Data Structures:** Define `structs` for tree nodes (`NODE_CMD`, `NODE_PIPE`, etc.).
-   [x] **Parser Implementation:** Write the main parser function that takes the token list and returns the root of the AST.
-   [x] **Parser - Simple Commands:** Handle commands without pipes or redirections (e.g., `ls -l`).
-   [x] **Parser - Pipes:** Handle the `|` operator, creating `NODE_PIPE` with left and right children.
-   [x] **Syntax Error Handling:** Detect and report syntax errors (e.g., `| ls` or `ls ||`) and prevent execution.
-   [x] **AST Printer:** Create a debug function to visually print the AST to verify parser logic.
-   [x] **Testing:** Verify the AST structure for commands like `cmd`, `cmd | cmd2`, `cmd1 | cmd2 | cmd3`.

### Phase 3: The Executor ✅
*   **Objective:** Execute simple commands from the AST.

-   [x] **Executor Function:** Create the main `execute()` function that recursively traverses the AST.
-   [x] **Command Execution:** For a `NODE_CMD`, use `fork()` to create a child process.
-   [x] **`execve` Integration:** In the child process, use `execve()` to run the command.
-   [x] **`waitpid` Integration:** In the parent process, use `waitpid()` to wait for the child to finish.
-   [x] **PATH Resolution:** Implement a helper function to find the full path of a command binary (e.g., `ls` -> `/bin/ls`).
-   [x] **Testing:** Ensure your shell can run basic external commands like `ls`, `wc`, `grep`, etc.

### Phase 4: Implementing Pipes ✅
*   **Objective:** Handle command pipelines by connecting stdout of one command to stdin of the next.

-   [x] **Pipe Execution:** Extend the executor to handle `NODE_PIPE`.
-   [x] **FD Management for Pipes:** Use `pipe()` and `dup2()` to connect the `stdout` of the left command to the `stdin` of the right command.
-   [x] **Close Unused FDs:** Meticulously close all unused file descriptors in parent and child processes to avoid hanging.
-   [x] **Multiple Pipes:** Handle complex pipe chains like `cmd1 | cmd2 | cmd3`.
-   [x] **Testing:** Test simple pipes (`ls | grep .c`) and complex chains (`ls | grep .c | wc -l`).

### Phase 4.5: Implementing Redirections ✅
*   **Objective:** Handle input/output redirection and here-documents.

-   [x] **Extend Lexer/Parser:** Add tokens and parsing logic for `<, >, >>, <<`.
-   [x] **Update AST:** Store redirection information within the `NODE_CMD` structure.
-   [x] **Output Redirection (`>`):** Implement `STDOUT_FILENO` redirection using `open()` and `dup2()`.
-   [x] **Append Redirection (`>>`):** Implement appending output.
-   [x] **Input Redirection (`<`):** Implement `STDIN_FILENO` redirection.
-   [x] **Heredoc (`<<`):** Implement logic to read from `stdin` until a delimiter is found, and use that as the command's input.
-   [x] **Testing:** Test each redirection type alone and combined with pipes (e.g., `cat < file1 | grep 'a' > file2`).

### Phase 5A: Environment Infrastructure
*   **Objective:** Build the foundation for variable expansion and environment management.

-   [ ] **Environment Storage System:** Create a data structure to store shell environment variables.
-   [ ] **Variable Expansion Engine:** Implement `$VAR` expansion in the lexer/parser phase.
-   [ ] **Exit Code Tracking:** Add global storage for the last command's exit code (`$?`).
-   [ ] **Quote-Aware Expansion:** Handle variable expansion differently in single vs double quotes.
-   [ ] **Testing:** Test `echo $HOME`, `echo $?`, `echo "$USER"`, `echo '$USER'`.

### Phase 5B: Built-ins Infrastructure
*   **Objective:** Create the framework for built-in command execution.

-   [ ] **Built-in Detection:** Add function to identify built-in commands (`echo`, `cd`, `pwd`, `env`, `export`, `unset`, `exit`).
-   [ ] **Process Context Handling:** Determine whether built-ins run in parent or child process.
-   [ ] **Built-in Function Registry:** Create a dispatch system for built-in commands.
-   [ ] **Environment Integration:** Connect built-ins to the environment system.
-   [ ] **Testing:** Test built-in detection and process context switching.

### Phase 5C: Built-ins Implementation
*   **Objective:** Implement individual built-in commands in dependency order.

-   [ ] **Implement `exit`:** Terminate the shell with optional exit code.
-   [ ] **Implement `env`:** Print all environment variables.
-   [ ] **Implement `export`:** Add/modify environment variables.
-   [ ] **Implement `unset`:** Remove environment variables.
-   [ ] **Implement `pwd`:** Print current working directory.
-   [ ] **Implement `cd`:** Change directory (requires `$HOME` expansion).
-   [ ] **Implement `echo`:** Print arguments with `-n` flag support (requires variable expansion).
-   [ ] **Testing:** Test each built-in individually and in combination.

### Phase 6: Signals and Interactive Features
*   **Objective:** Make the shell responsive to signals and properly interactive.

-   [ ] **Signal `SIGINT` (`Ctrl-C`):** Intercept in main shell and running commands differently.
-   [ ] **Signal `SIGQUIT` (`Ctrl-\`):** Handle appropriately in shell vs commands.
-   [ ] **Signal Integration:** Ensure signals work with pipes and redirections.
-   [ ] **Prompt Management:** Show new prompt after signal interruption.
-   [ ] **Testing:** Test signals with and without running commands.

### Phase 7: Final Polish and Edge Case Testing
*   **Objective:** Ensure robustness and compliance with all requirements.

-   [ ] **Memory Leak Hunting:** Run under `valgrind` and fix all leaks.
-   [ ] **Complex Quoting:** Test edge cases like `echo "'$USER'"` vs `echo '"$USER"'`.
-   [ ] **Empty Input Handling:** Handle empty lines gracefully.
-   [ ] **Error Message Standardization:** Match `bash` error formats.
-   [ ] **Edge Case Testing:** Test unusual combinations of features.
-   [ ] **Code Review:** Final cleanup and documentation.
-   [ ] **Final Submission:** Project complete.
