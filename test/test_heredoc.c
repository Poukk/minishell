#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "minishell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

// Test heredoc creation function
Test(heredoc_unit_tests, test_heredoc_redirection_create) {
	t_gc gc;
	t_redirection *redir;
	
	gc_init(&gc);
	
	redir = heredoc_redirection_create(&gc, "EOF", "hello\nworld\n");
	
	cr_assert_not_null(redir);
	cr_assert_eq(redir->type, TOKEN_HEREDOC);
	cr_assert_str_eq(redir->filename, "EOF");
	cr_assert_str_eq(redir->heredoc_content, "hello\nworld\n");
	cr_assert_null(redir->next);
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_heredoc_redirection_create_null_content) {
	t_gc gc;
	t_redirection *redir;
	
	gc_init(&gc);
	
	redir = heredoc_redirection_create(&gc, "EOF", NULL);
	
	cr_assert_not_null(redir);
	cr_assert_eq(redir->type, TOKEN_HEREDOC);
	cr_assert_str_eq(redir->filename, "EOF");
	cr_assert_null(redir->heredoc_content);
	cr_assert_null(redir->next);
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_heredoc_redirection_create_empty_content) {
	t_gc gc;
	t_redirection *redir;
	
	gc_init(&gc);
	
	redir = heredoc_redirection_create(&gc, "EOF", "");
	
	cr_assert_not_null(redir);
	cr_assert_eq(redir->type, TOKEN_HEREDOC);
	cr_assert_str_eq(redir->filename, "EOF");
	cr_assert_str_eq(redir->heredoc_content, "");
	cr_assert_null(redir->next);
	
	gc_free_all(&gc);
}

// Test variable expansion function
Test(heredoc_unit_tests, test_expand_heredoc_variables_no_expansion) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	result = expand_heredoc_variables(&gc, "hello world", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "hello world");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_quoted_delimiter) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	// When delimiter is quoted, no expansion should occur
	result = expand_heredoc_variables(&gc, "hello $HOME world", "'EOF'");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "hello $HOME world");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_double_quoted_delimiter) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	// When delimiter is quoted, no expansion should occur
	result = expand_heredoc_variables(&gc, "hello $USER world", "\"EOF\"");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "hello $USER world");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_with_expansion) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	setenv("TEST_VAR", "expanded", 1);
	
	result = expand_heredoc_variables(&gc, "hello $TEST_VAR world", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "hello expanded world");
	
	unsetenv("TEST_VAR");
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_undefined_var) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	unsetenv("UNDEFINED_VAR");
	
	result = expand_heredoc_variables(&gc, "hello $UNDEFINED_VAR world", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "hello  world");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_multiple_vars) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	setenv("VAR1", "first", 1);
	setenv("VAR2", "second", 1);
	
	result = expand_heredoc_variables(&gc, "$VAR1 and $VAR2", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "first and second");
	
	unsetenv("VAR1");
	unsetenv("VAR2");
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_dollar_at_end) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	result = expand_heredoc_variables(&gc, "price is 5$", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "price is 5$");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_empty_string) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	result = expand_heredoc_variables(&gc, "", "EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, "");
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_expand_heredoc_variables_null_input) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	
	result = expand_heredoc_variables(&gc, NULL, "EOF");
	
	cr_assert_null(result);
	
	gc_free_all(&gc);
}

// Test heredoc setup redirection
Test(heredoc_unit_tests, test_setup_heredoc_redirection_null_redir) {
	int result = setup_heredoc_redirection(NULL);
	cr_assert_eq(result, 0);
}

Test(heredoc_unit_tests, test_setup_heredoc_redirection_empty_content) {
	t_gc gc;
	t_redirection *redir;
	int result;
	int saved_stdin;
	
	gc_init(&gc);
	saved_stdin = dup(STDIN_FILENO);
	
	redir = heredoc_redirection_create(&gc, "EOF", "");
	result = setup_heredoc_redirection(redir);
	
	cr_assert_eq(result, 0);
	
	// Restore stdin
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	
	gc_free_all(&gc);
}

Test(heredoc_unit_tests, test_setup_heredoc_redirection_with_content) {
	t_gc gc;
	t_redirection *redir;
	int result;
	int saved_stdin;
	char buffer[1024];
	ssize_t bytes_read;
	
	gc_init(&gc);
	saved_stdin = dup(STDIN_FILENO);
	
	redir = heredoc_redirection_create(&gc, "EOF", "hello\nworld\n");
	result = setup_heredoc_redirection(redir);
	
	cr_assert_eq(result, 0);
	
	// Read from stdin to verify content was written
	bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	buffer[bytes_read] = '\0';
	
	cr_assert_str_eq(buffer, "hello\nworld\n");
	
	// Restore stdin
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	
	gc_free_all(&gc);
}

// Integration test for variable expansion (no parsing)
Test(heredoc_integration, test_heredoc_with_variable_expansion_complex) {
	t_gc gc;
	char *result;
	
	gc_init(&gc);
	setenv("USER", "testuser", 1);
	setenv("HOME", "/home/testuser", 1);
	setenv("PATH", "/usr/bin:/bin", 1);
	
	result = expand_heredoc_variables(&gc, 
		"Welcome $USER!\n"
		"Your home is: $HOME\n"
		"PATH: $PATH\n"
		"Dollar sign: $$\n"
		"End of message.", 
		"EOF");
	
	cr_assert_not_null(result);
	cr_assert_str_eq(result, 
		"Welcome testuser!\n"
		"Your home is: /home/testuser\n"
		"PATH: /usr/bin:/bin\n"
		"Dollar sign: $\n"
		"End of message.");
	
	unsetenv("USER");
	unsetenv("HOME");
	unsetenv("PATH");
	gc_free_all(&gc);
}

Test(heredoc_stress, test_heredoc_very_large_content) {
	t_gc gc;
	t_redirection *redir;
	char *large_content;
	size_t content_size = 100000; // 100KB content
	size_t i;
	
	gc_init(&gc);
	
	large_content = gc_malloc(&gc, content_size + 1);
	cr_assert_not_null(large_content);
	
	// Fill with repeated pattern
	for (i = 0; i < content_size; i++) {
		large_content[i] = 'A' + (i % 26);
	}
	large_content[content_size] = '\0';
	
	redir = heredoc_redirection_create(&gc, "LARGE_EOF", large_content);
	
	cr_assert_not_null(redir);
	cr_assert_eq(redir->type, TOKEN_HEREDOC);
	cr_assert_str_eq(redir->filename, "LARGE_EOF");
	cr_assert_eq(strlen(redir->heredoc_content), content_size);
	
	gc_free_all(&gc);
}