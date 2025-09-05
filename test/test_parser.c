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
