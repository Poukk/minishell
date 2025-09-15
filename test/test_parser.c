#include <criterion/criterion.h>
#include "minishell.h"

Test(parser_tests, test_simple_command) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "ls -l");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "ls");
	cr_assert_str_eq(ast->args[1], "-l");
	cr_assert_null(ast->args[2]);
	cr_assert_null(ast->left);
	cr_assert_null(ast->right);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_simple_pipe) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "ls | grep txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_PIPE);
	cr_assert_not_null(ast->left);
	cr_assert_not_null(ast->right);
	
	cr_assert_eq(ast->left->type, NODE_CMD);
	cr_assert_str_eq(ast->left->args[0], "ls");
	
	cr_assert_eq(ast->right->type, NODE_CMD);
	cr_assert_str_eq(ast->right->args[0], "grep");
	cr_assert_str_eq(ast->right->args[1], "txt");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_multiple_pipes) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "cat file | grep pattern | wc -l");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_PIPE);
	cr_assert_not_null(ast->left);
	cr_assert_not_null(ast->right);
	
	cr_assert_eq(ast->left->type, NODE_CMD);
	cr_assert_str_eq(ast->left->args[0], "cat");
	cr_assert_str_eq(ast->left->args[1], "file");
	
	cr_assert_eq(ast->right->type, NODE_PIPE);
	cr_assert_not_null(ast->right->left);
	cr_assert_not_null(ast->right->right);
	
	cr_assert_eq(ast->right->left->type, NODE_CMD);
	cr_assert_str_eq(ast->right->left->args[0], "grep");
	cr_assert_str_eq(ast->right->left->args[1], "pattern");
	
	cr_assert_eq(ast->right->right->type, NODE_CMD);
	cr_assert_str_eq(ast->right->right->args[0], "wc");
	cr_assert_str_eq(ast->right->right->args[1], "-l");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_syntax_error_pipe_at_start) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "| ls");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_null(ast);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_syntax_error_double_pipe) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "ls ||");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_null(ast);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_syntax_error_empty_command_between_pipes) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "ls | | grep");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_null(ast);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_pipes_with_redirections) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "ls | echo hello > file.txt | cat file.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_PIPE);
	cr_assert_not_null(ast->left);
	cr_assert_not_null(ast->right);
	
	cr_assert_eq(ast->left->type, NODE_CMD);
	cr_assert_str_eq(ast->left->args[0], "ls");
	
	cr_assert_eq(ast->right->type, NODE_PIPE);
	cr_assert_not_null(ast->right->left);
	cr_assert_not_null(ast->right->right);
	
	cr_assert_eq(ast->right->left->type, NODE_CMD);
	cr_assert_str_eq(ast->right->left->args[0], "echo");
	cr_assert_str_eq(ast->right->left->args[1], "hello");
	
	cr_assert_eq(ast->right->right->type, NODE_CMD);
	cr_assert_str_eq(ast->right->right->args[0], "cat");
	cr_assert_str_eq(ast->right->right->args[1], "file.txt");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_input_redirection) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "cat < input.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "cat");
	cr_assert_null(ast->args[1]);
	
	cr_assert_not_null(ast->input_redirs);
	cr_assert_eq(ast->input_redirs->type, TOKEN_REDIR_IN);
	cr_assert_str_eq(ast->input_redirs->filename, "input.txt");
	cr_assert_null(ast->output_redirs);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_output_redirection) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "echo hello > output.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "echo");
	cr_assert_str_eq(ast->args[1], "hello");
	cr_assert_null(ast->args[2]);
	
	cr_assert_null(ast->input_redirs);
	cr_assert_not_null(ast->output_redirs);
	cr_assert_eq(ast->output_redirs->type, TOKEN_REDIR_OUT);
	cr_assert_str_eq(ast->output_redirs->filename, "output.txt");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_both_redirections) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "cat < input.txt > output.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "cat");
	cr_assert_null(ast->args[1]);
	
	cr_assert_not_null(ast->input_redirs);
	cr_assert_eq(ast->input_redirs->type, TOKEN_REDIR_IN);
	cr_assert_str_eq(ast->input_redirs->filename, "input.txt");
	
	cr_assert_not_null(ast->output_redirs);
	cr_assert_eq(ast->output_redirs->type, TOKEN_REDIR_OUT);
	cr_assert_str_eq(ast->output_redirs->filename, "output.txt");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_redirections_mixed_with_args) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "grep pattern < input.txt > output.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "grep");
	cr_assert_str_eq(ast->args[1], "pattern");
	cr_assert_null(ast->args[2]);
	
	cr_assert_not_null(ast->input_redirs);
	cr_assert_str_eq(ast->input_redirs->filename, "input.txt");
	
	cr_assert_not_null(ast->output_redirs);
	cr_assert_str_eq(ast->output_redirs->filename, "output.txt");
	
	gc_free_all(&gc);
}

Test(parser_tests, test_multiple_output_redirections) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "echo hello > file1.txt > file2.txt > file3.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "echo");
	cr_assert_str_eq(ast->args[1], "hello");
	cr_assert_null(ast->args[2]);
	
	cr_assert_null(ast->input_redirs);
	cr_assert_not_null(ast->output_redirs);
	
	// Should have 3 output redirections in the list
	t_redirection *current = ast->output_redirs;
	cr_assert_not_null(current);
	cr_assert_str_eq(current->filename, "file1.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_str_eq(current->filename, "file2.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_str_eq(current->filename, "file3.txt");
	
	current = current->next;
	cr_assert_null(current);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_simple_append_redirection) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "echo hello >> output.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "echo");
	cr_assert_str_eq(ast->args[1], "hello");
	cr_assert_null(ast->args[2]);
	
	cr_assert_null(ast->input_redirs);
	cr_assert_not_null(ast->output_redirs);
	cr_assert_eq(ast->output_redirs->type, TOKEN_REDIR_APPEND);
	cr_assert_str_eq(ast->output_redirs->filename, "output.txt");
	cr_assert_null(ast->output_redirs->next);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_multiple_append_redirections) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "echo hello >> file1.txt >> file2.txt >> file3.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "echo");
	cr_assert_str_eq(ast->args[1], "hello");
	cr_assert_null(ast->args[2]);
	
	cr_assert_null(ast->input_redirs);
	cr_assert_not_null(ast->output_redirs);
	
	// Should have 3 append redirections in the list
	t_redirection *current = ast->output_redirs;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_APPEND);
	cr_assert_str_eq(current->filename, "file1.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_APPEND);
	cr_assert_str_eq(current->filename, "file2.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_APPEND);
	cr_assert_str_eq(current->filename, "file3.txt");
	
	current = current->next;
	cr_assert_null(current);
	
	gc_free_all(&gc);
}

Test(parser_tests, test_mixed_truncate_append_redirections) {
	t_gc gc;
	t_token *tokens;
	t_ast_node *ast;
	
	gc_init(&gc);
	tokens = lexer_tokenize(&gc, "echo hello > file1.txt >> file2.txt > file3.txt");
	ast = parser_parse(&gc, tokens);
	
	cr_assert_not_null(ast);
	cr_assert_eq(ast->type, NODE_CMD);
	cr_assert_not_null(ast->args);
	cr_assert_str_eq(ast->args[0], "echo");
	cr_assert_str_eq(ast->args[1], "hello");
	cr_assert_null(ast->args[2]);
	
	cr_assert_null(ast->input_redirs);
	cr_assert_not_null(ast->output_redirs);
	
	// Should have mixed redirections: > >> >
	t_redirection *current = ast->output_redirs;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_OUT);
	cr_assert_str_eq(current->filename, "file1.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_APPEND);
	cr_assert_str_eq(current->filename, "file2.txt");
	
	current = current->next;
	cr_assert_not_null(current);
	cr_assert_eq(current->type, TOKEN_REDIR_OUT);
	cr_assert_str_eq(current->filename, "file3.txt");
	
	current = current->next;
	cr_assert_null(current);
	
	gc_free_all(&gc);
}
