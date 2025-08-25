#include <criterion/criterion.h>
#include "minishell.h"

Test(lexer_tests, test_empty_input) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "");
    
    cr_assert_not_null(tokens, "Expected tokens to be created for empty input");
    cr_assert_eq(tokens->type, TOKEN_EOF, "Expected EOF token for empty input");
    cr_assert_null(tokens->next, "Expected only one token for empty input");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_whitespace_only) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "   \t  ");
    
    cr_assert_not_null(tokens, "Expected tokens to be created for whitespace-only input");
    cr_assert_eq(tokens->type, TOKEN_EOF, "Expected EOF token for whitespace-only input");
    cr_assert_null(tokens->next, "Expected only one token for whitespace-only input");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_simple_command) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "ls");
    
    cr_assert_not_null(tokens, "Expected tokens to be created");
    cr_assert_eq(tokens->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(tokens->value, "ls", "Expected token value to be 'ls'");
    
    cr_assert_not_null(tokens->next, "Expected EOF token after command");
    cr_assert_eq(tokens->next->type, TOKEN_EOF, "Expected second token to be EOF");
    cr_assert_null(tokens->next->next, "Expected only two tokens");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_command_with_args) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "ls -la");
    
    current = tokens;
    cr_assert_not_null(current, "Expected first token");
    cr_assert_eq(current->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(current->value, "ls", "Expected first token value to be 'ls'");
    
    current = current->next;
    cr_assert_not_null(current, "Expected second token");
    cr_assert_eq(current->type, TOKEN_WORD, "Expected second token to be WORD");
    cr_assert_str_eq(current->value, "-la", "Expected second token value to be '-la'");
    
    current = current->next;
    cr_assert_not_null(current, "Expected EOF token");
    cr_assert_eq(current->type, TOKEN_EOF, "Expected third token to be EOF");
    cr_assert_null(current->next, "Expected only three tokens");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_pipe_simple) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "ls | wc");
    
    current = tokens;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(current->value, "ls", "Expected first token value to be 'ls'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_PIPE, "Expected second token to be PIPE");
    cr_assert_str_eq(current->value, "|", "Expected pipe token value to be '|'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected third token to be WORD");
    cr_assert_str_eq(current->value, "wc", "Expected third token value to be 'wc'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_EOF, "Expected fourth token to be EOF");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_redirections) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "cat < input > output");
    
    current = tokens;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(current->value, "cat", "Expected first token value to be 'cat'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_REDIR_IN, "Expected second token to be REDIR_IN");
    cr_assert_str_eq(current->value, "<", "Expected redirect in token value to be '<'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected third token to be WORD");
    cr_assert_str_eq(current->value, "input", "Expected third token value to be 'input'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_REDIR_OUT, "Expected fourth token to be REDIR_OUT");
    cr_assert_str_eq(current->value, ">", "Expected redirect out token value to be '>'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected fifth token to be WORD");
    cr_assert_str_eq(current->value, "output", "Expected fifth token value to be 'output'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_EOF, "Expected sixth token to be EOF");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_append_redirect) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "echo hello >> file");
    
    current = tokens;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected first token to be WORD");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected second token to be WORD");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_REDIR_APPEND, "Expected third token to be REDIR_APPEND");
    cr_assert_str_eq(current->value, ">>", "Expected append token value to be '>>'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected fourth token to be WORD");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_heredoc) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "cat << EOF");
    
    current = tokens;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected first token to be WORD");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_HEREDOC, "Expected second token to be HEREDOC");
    cr_assert_str_eq(current->value, "<<", "Expected heredoc token value to be '<<'");
    
    current = current->next;
    cr_assert_eq(current->type, TOKEN_WORD, "Expected third token to be WORD");
    cr_assert_str_eq(current->value, "EOF", "Expected third token value to be 'EOF'");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_single_quotes) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "echo 'hello world'");
    
    cr_assert_eq(tokens->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(tokens->value, "echo", "Expected first token value to be 'echo'");
    
    cr_assert_eq(tokens->next->type, TOKEN_WORD, "Expected second token to be WORD");
    cr_assert_str_eq(tokens->next->value, "hello world", "Expected quoted string to preserve spaces");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_double_quotes) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "echo \"hello world\"");
    
    cr_assert_eq(tokens->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(tokens->value, "echo", "Expected first token value to be 'echo'");
    
    cr_assert_eq(tokens->next->type, TOKEN_WORD, "Expected second token to be WORD");
    cr_assert_str_eq(tokens->next->value, "hello world", "Expected quoted string to preserve spaces");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_multiple_spaces) {
    t_gc gc;
    t_token *tokens;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "ls    -la");
    
    cr_assert_eq(tokens->type, TOKEN_WORD, "Expected first token to be WORD");
    cr_assert_str_eq(tokens->value, "ls", "Expected first token value to be 'ls'");
    
    cr_assert_eq(tokens->next->type, TOKEN_WORD, "Expected second token to be WORD");
    cr_assert_str_eq(tokens->next->value, "-la", "Expected second token value to be '-la'");
    
    cr_assert_eq(tokens->next->next->type, TOKEN_EOF, "Expected third token to be EOF");
    
    gc_free_all(&gc);
}

Test(lexer_tests, test_complex_pipeline) {
    t_gc gc;
    t_token *tokens;
    t_token *current;
    int token_count = 0;
    
    gc_init(&gc);
    tokens = lexer_tokenize(&gc, "cat file | grep test | wc -l");
    
    current = tokens;
    while (current && current->type != TOKEN_EOF) {
        token_count++;
        current = current->next;
    }
    
    cr_assert_eq(token_count, 8, "Expected 8 tokens before EOF for complex pipeline");
    
    gc_free_all(&gc);
}