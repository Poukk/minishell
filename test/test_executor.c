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

Test(executor_tests, test_executor_pipe_not_implemented) {
	t_gc gc;
	t_ast_node *node;
	int result;
	
	gc_init(&gc);
	node = ast_node_create(&gc, NODE_PIPE);
	
	result = executor_execute(node);
	cr_assert_eq(result, 1);
	
	gc_free_all(&gc);
}