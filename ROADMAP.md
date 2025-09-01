# Minishell Roadmap

Use this checklist to track progress and identify next steps.

### Phase 1: The Core Loop, Lexer, and Scoped Allocator
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

### Phase 2: The Parser and Abstract Syntax Tree (AST)
*   **Objective:** Convert the flat list of tokens into a hierarchical command tree (AST).

-   [x] **AST Data Structures:** Define `structs` for tree nodes (`NODE_CMD`, `NODE_PIPE`, etc.).
-   [x] **Parser Implementation:** Write the main parser function that takes the token list and returns the root of the AST.
-   [ ] **Parser - Simple Commands:** Handle commands without pipes or redirections (e.g., `ls -l`).
-   [ ] **Parser - Pipes:** Handle the `|` operator, creating `NODE_PIPE` with left and right children.
-   [ ] **Syntax Error Handling:** Detect and report syntax errors (e.g., `| ls` or `ls ||`) and prevent execution.
-   [ ] **AST Printer:** Create a debug function to visually print the AST to verify parser logic.
-   [ ] **Testing:** Verify the AST structure for commands like `cmd`, `cmd | cmd2`, `cmd1 | cmd2 | cmd3`.

### Phase 3: The Executor
*   **Objective:** Execute simple commands from the AST.

-   [ ] **Executor Function:** Create the main `execute()` function that recursively traverses the AST.
-   [ ] **Command Execution:** For a `NODE_CMD`, use `fork()` to create a child process.
-   [ ] **`execve` Integration:** In the child process, use `execve()` to run the command.
-   [ ] **`waitpid` Integration:** In the parent process, use `waitpid()` to wait for the child to finish.
-   [ ] **PATH Resolution:** Implement a helper function to find the full path of a command binary (e.g., `ls` -> `/bin/ls`).
-   [ ] **Testing:** Ensure your shell can run basic external commands like `ls`, `wc`, `grep`, etc.

### Phase 4: Implementing Pipes and Built-ins
*   **Objective:** Handle command pipelines and shell built-in commands.

-   [ ] **Pipe Execution:** Extend the executor to handle `NODE_PIPE`.
-   [ ] **FD Management for Pipes:** Use `pipe()` and `dup2()` to connect the `stdout` of the left command to the `stdin` of the right command.
-   [ ] **Close Unused FDs:** Meticulously close all unused file descriptors in parent and child processes to avoid hanging.
-   [ ] **Identify Built-ins:** Determine which commands are built-ins (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`).
-   [ ] **Implement `echo`:** Handle the `-n` flag.
-   [ ] **Implement `cd`:** Change the current working directory.
-   [ ] **Implement `pwd`:** Print the current working directory.
-   [ ] **Implement `env`:** Print environment variables.
-   [ ] **Implement `export` and `unset`:** Manage environment variables.
-   [ ] **Implement `exit`:** Terminate the shell.
-   [ ] **Built-in Execution:** Add logic to the executor to run built-ins in the correct process (parent for `cd`, `export`, `unset`, `exit`).

### Phase 5: Implementing Redirections
*   **Objective:** Handle input/output redirection and here-documents.

-   [ ] **Extend Lexer/Parser:** Add tokens and parsing logic for `<, >, >>, <<`.
-   [ ] **Update AST:** Store redirection information within the `NODE_CMD` structure.
-   [ ] **Output Redirection (`>`):** Implement `STDOUT_FILENO` redirection using `open()` and `dup2()`.
-   [ ] **Append Redirection (`>>`):** Implement appending output.
-   [ ] **Input Redirection (`<`):** Implement `STDIN_FILENO` redirection.
-   [ ] **Heredoc (`<<`):** Implement logic to read from `stdin` until a delimiter is found, and use that as the command's input.
-   [ ] **Testing:** Test each redirection type alone and combined with pipes (e.g., `cat < file1 | grep 'a' > file2`).

### Phase 6: Signals, Environment, and Exit Codes
*   **Objective:** Make the shell interactive, responsive, and stateful.

-   [ ] **Signal `SIGINT` (`Ctrl-C`):** Intercept the signal. It should not kill the shell, but should terminate a running child process and show a new prompt.
-   [ ] **Signal `SIGQUIT` (`Ctrl-\`):** Intercept the signal. It should do nothing in the main shell, but quit a running child process.
-   [ ] **Environment Variable Expansion:** Before execution, parse and expand variables like `$USER` or `$PATH`.
-   [ ] **Exit Code Expansion (`$?`):** Implement expansion for the exit code of the most recent command.
-   [ ] **Exit Code Storage:** Store the exit code from `waitpid()` after each command execution.
-   [ ] **Testing:** Test signals with and without a running command. Test `echo $?` after success and failure. Test variable expansion with quotes.

### Phase 7: Final Polish and Edge Case Testing
*   **Objective:** Ensure the project is robust, leak-free, and compliant with all requirements.

-   [ ] **Leak Hunting:** Run the shell under `valgrind` or with `leaks` and fix every single memory leak.
-   [ ] **Quote Hell:** Rigorously test complex quoting scenarios (e.g., `echo "'$USER'"` vs `echo '"$USER"'`).
-   [ ] **Empty Command:** Handle the user hitting Enter on an empty line.
-   [ ] **Error Messages:** Ensure all error messages are printed to `stderr` and match `bash` format where applicable.
-   [ ] **Code Review:** Do a final self-review of the entire codebase.
-   [ ] **Final Submission:** The project is complete.
