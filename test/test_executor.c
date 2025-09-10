#include <criterion/criterion.h>
#include "minishell.h"

Test(executor_tests, test_path_resolution_absolute) {
	char *result;
	
	result = resolve_command_path("/bin/ls");
	if (result) {
		cr_assert_str_eq(result, "/bin/ls");
		free(result);
	}
}

Test(executor_tests, test_path_resolution_relative) {
	char *result;
	
	result = resolve_command_path("./minishell");
	if (result) {
		cr_assert_str_eq(result, "./minishell");
		free(result);
	}
}

Test(executor_tests, test_path_resolution_in_path) {
	char *result;
	
	result = resolve_command_path("ls");
	cr_assert_not_null(result);
	if (result) {
		free(result);
	}
}

Test(executor_tests, test_path_resolution_invalid) {
	char *result;
	
	result = resolve_command_path("nonexistent_command_xyz");
	cr_assert_null(result);
}

Test(executor_tests, test_executor_simple_command) {
	t_gc gc;
	t_ast_node *node;
	char *args[] = {"echo", "test", NULL};
	int result;
	
	gc_init(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, node, args);
	
	result = executor_execute(node);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_executor_invalid_command) {
	t_gc gc;
	t_ast_node *node;
	char *args[] = {"nonexistent_xyz_cmd", NULL};
	int result;
	
	gc_init(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, node, args);
	
	result = executor_execute(node);
	cr_assert_eq(result, 127);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_simple_pipe) {
	t_gc gc;
	t_ast_node *pipe_node, *left_cmd, *right_cmd;
	char *left_args[] = {"echo", "hello", NULL};
	char *right_args[] = {"cat", NULL};
	int result;
	
	gc_init(&gc);
	
	pipe_node = ast_node_create(&gc, NODE_PIPE);
	left_cmd = ast_node_create(&gc, NODE_CMD);
	right_cmd = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left_cmd, left_args);
	ast_node_set_args(&gc, right_cmd, right_args);
	
	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;
	
	result = executor_execute(pipe_node);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_pipe_with_grep) {
	t_gc gc;
	t_ast_node *pipe_node, *left_cmd, *right_cmd;
	char *left_args[] = {"echo", "hello\nworld\nhello", NULL};
	char *right_args[] = {"grep", "hello", NULL};
	int result;
	
	gc_init(&gc);
	
	pipe_node = ast_node_create(&gc, NODE_PIPE);
	left_cmd = ast_node_create(&gc, NODE_CMD);
	right_cmd = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left_cmd, left_args);
	ast_node_set_args(&gc, right_cmd, right_args);
	
	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;
	
	result = executor_execute(pipe_node);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_multiple_pipes) {
	t_gc gc;
	t_ast_node *outer_pipe, *inner_pipe, *cmd1, *cmd2, *cmd3;
	char *args1[] = {"echo", "test", NULL};
	char *args2[] = {"cat", NULL};
	char *args3[] = {"wc", "-l", NULL};
	int result;
	
	gc_init(&gc);
	
	outer_pipe = ast_node_create(&gc, NODE_PIPE);
	inner_pipe = ast_node_create(&gc, NODE_PIPE);
	cmd1 = ast_node_create(&gc, NODE_CMD);
	cmd2 = ast_node_create(&gc, NODE_CMD);
	cmd3 = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, cmd1, args1);
	ast_node_set_args(&gc, cmd2, args2);
	ast_node_set_args(&gc, cmd3, args3);
	
	inner_pipe->left = cmd1;
	inner_pipe->right = cmd2;
	outer_pipe->left = inner_pipe;
	outer_pipe->right = cmd3;
	
	result = executor_execute(outer_pipe);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}