#include <criterion/criterion.h>
#include "minishell.h"

Test(ast_tests, test_node_creation) {
	t_gc gc;
	t_ast_node *node;
	
	gc_init(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	
	cr_assert_not_null(node);
	cr_assert_eq(node->type, NODE_CMD);
	cr_assert_null(node->args);
	cr_assert_null(node->left);
	cr_assert_null(node->right);
	
	gc_free_all(&gc);
}

Test(ast_tests, test_node_with_args) {
	t_gc gc;
	t_ast_node *node;
	char *args[] = {"ls", "-l", NULL};
	
	gc_init(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, node, args);
	
	cr_assert_not_null(node);
	cr_assert_not_null(node->args);
	cr_assert_str_eq(node->args[0], "ls");
	cr_assert_str_eq(node->args[1], "-l");
	cr_assert_null(node->args[2]);
	
	gc_free_all(&gc);
}

Test(ast_tests, test_pipe_node) {
	t_gc gc;
	t_ast_node *root;
	t_ast_node *left;
	t_ast_node *right;
	char *args1[] = {"ls", NULL};
	char *args2[] = {"grep", "txt", NULL};
	
	gc_init(&gc);
	root = ast_node_create(&gc, NODE_PIPE);
	left = ast_node_create(&gc, NODE_CMD);
	right = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left, args1);
	ast_node_set_args(&gc, right, args2);
	
	root->left = left;
	root->right = right;
	
	cr_assert_not_null(root);
	cr_assert_eq(root->type, NODE_PIPE);
	cr_assert_not_null(root->left);
	cr_assert_not_null(root->right);
	cr_assert_str_eq(root->left->args[0], "ls");
	cr_assert_str_eq(root->right->args[0], "grep");
	
	gc_free_all(&gc);
}