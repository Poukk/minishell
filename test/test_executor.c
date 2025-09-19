#include <criterion/criterion.h>
#include "minishell.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static t_shell_context *create_test_context(t_gc *gc) {
	t_shell_context *ctx;
	
	ctx = gc_malloc(gc, sizeof(t_shell_context));
	if (!ctx)
		return (NULL);
	ctx->env = env_create(gc);
	ctx->gc = gc;
	return (ctx);
}

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
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, node, args);
	
	result = executor_execute(node, ctx);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_executor_invalid_command) {
	t_gc gc;
	t_ast_node *node;
	char *args[] = {"nonexistent_xyz_cmd", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, node, args);
	
	result = executor_execute(node, ctx);
	cr_assert_eq(result, 127);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_simple_pipe) {
	t_gc gc;
	t_ast_node *pipe_node, *left_cmd, *right_cmd;
	char *left_args[] = {"echo", "hello", NULL};
	char *right_args[] = {"cat", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	pipe_node = ast_node_create(&gc, NODE_PIPE);
	left_cmd = ast_node_create(&gc, NODE_CMD);
	right_cmd = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left_cmd, left_args);
	ast_node_set_args(&gc, right_cmd, right_args);
	
	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;
	
	result = executor_execute(pipe_node, ctx);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_pipe_with_grep) {
	t_gc gc;
	t_ast_node *pipe_node, *left_cmd, *right_cmd;
	char *left_args[] = {"echo", "hello\nworld\nhello", NULL};
	char *right_args[] = {"grep", "hello", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	pipe_node = ast_node_create(&gc, NODE_PIPE);
	left_cmd = ast_node_create(&gc, NODE_CMD);
	right_cmd = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left_cmd, left_args);
	ast_node_set_args(&gc, right_cmd, right_args);
	
	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;
	
	result = executor_execute(pipe_node, ctx);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_multiple_pipes) {
	t_gc gc;
	t_ast_node *outer_pipe, *inner_pipe, *cmd1, *cmd2, *cmd3;
	char *args1[] = {"echo", "test", NULL};
	char *args2[] = {"cat", NULL};
	char *args3[] = {"wc", "-l", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
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
	
	result = executor_execute(outer_pipe, ctx);
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(executor_tests, test_output_redirection) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "hello world", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->output_redirs = redirection_create(&gc, TOKEN_REDIR_OUT, 
											   "test_output.txt");
	
	result = executor_execute(cmd_node, ctx);
	cr_assert_eq(result, 0);
	
	int fd = open("test_output.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[50];
		int bytes_read = read(fd, buffer, 49);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			cr_assert_str_eq(buffer, "hello world\n");
		}
		close(fd);
		unlink("test_output.txt");
	}
	
	gc_free_all(&gc);
}

Test(executor_tests, test_input_redirection) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"cat", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	int fd = open("test_input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1) {
		write(fd, "test content\n", 13);
		close(fd);
	}
	
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->input_redirs = redirection_create(&gc, TOKEN_REDIR_IN, 
											  "test_input.txt");
	
	result = executor_execute(cmd_node, ctx);
	cr_assert_eq(result, 0);
	
	unlink("test_input.txt");
	
	gc_free_all(&gc);
}

Test(executor_tests, test_both_redirections) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"cat", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	int fd = open("test_input2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd != -1) {
		write(fd, "input data\n", 11);
		close(fd);
	}
	
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->input_redirs = redirection_create(&gc, TOKEN_REDIR_IN, 
											  "test_input2.txt");
	cmd_node->output_redirs = redirection_create(&gc, TOKEN_REDIR_OUT, 
											   "test_output2.txt");
	
	result = executor_execute(cmd_node, ctx);
	cr_assert_eq(result, 0);
	
	fd = open("test_output2.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[50];
		int bytes_read = read(fd, buffer, 49);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			cr_assert_str_eq(buffer, "input data\n");
		}
		close(fd);
		unlink("test_output2.txt");
	}
	
	unlink("test_input2.txt");
	
	gc_free_all(&gc);
}

Test(executor_tests, test_pipe_with_redirection) {
	t_gc gc;
	t_ast_node *pipe_node, *left_cmd, *right_cmd;
	char *left_args[] = {"echo", "hello world", NULL};
	char *right_args[] = {"cat", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	pipe_node = ast_node_create(&gc, NODE_PIPE);
	left_cmd = ast_node_create(&gc, NODE_CMD);
	right_cmd = ast_node_create(&gc, NODE_CMD);
	
	ast_node_set_args(&gc, left_cmd, left_args);
	ast_node_set_args(&gc, right_cmd, right_args);
	
	right_cmd->output_redirs = redirection_create(&gc, TOKEN_REDIR_OUT, 
												 "pipe_test_output.txt");
	
	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;
	
	result = executor_execute(pipe_node, ctx);
	cr_assert_eq(result, 0);
	
	int fd = open("pipe_test_output.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[50];
		int bytes_read = read(fd, buffer, 49);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			cr_assert_str_eq(buffer, "hello world\n");
		}
		close(fd);
		unlink("pipe_test_output.txt");
	}
	
	gc_free_all(&gc);
}

Test(executor_tests, test_multiple_output_redirections) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "hello", NULL};
	t_shell_context *ctx;
	int result;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	
	// Create a chain of 3 output redirections
	t_redirection *redir1 = redirection_create(&gc, TOKEN_REDIR_OUT, "test_file1.txt");
	t_redirection *redir2 = redirection_create(&gc, TOKEN_REDIR_OUT, "test_file2.txt");
	t_redirection *redir3 = redirection_create(&gc, TOKEN_REDIR_OUT, "test_file3.txt");
	
	redir1->next = redir2;
	redir2->next = redir3;
	cmd_node->output_redirs = redir1;
	
	result = executor_execute(cmd_node, ctx);
	cr_assert_eq(result, 0);
	
	// All three files should be created
	cr_assert_eq(access("test_file1.txt", F_OK), 0);
	cr_assert_eq(access("test_file2.txt", F_OK), 0);
	cr_assert_eq(access("test_file3.txt", F_OK), 0);
	
	// Only the last file should contain the output
	int fd = open("test_file3.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[50];
		int bytes_read = read(fd, buffer, 49);
		if (bytes_read > 0) {
			buffer[bytes_read] = '\0';
			cr_assert_str_eq(buffer, "hello\n");
		}
		close(fd);
	}
	
	// The first two files should be empty (created but not written to)
	fd = open("test_file1.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[10];
		int bytes_read = read(fd, buffer, 9);
		cr_assert_eq(bytes_read, 0); // Should be empty
		close(fd);
	}
	
	fd = open("test_file2.txt", O_RDONLY);
	if (fd != -1) {
		char buffer[10];
		int bytes_read = read(fd, buffer, 9);
		cr_assert_eq(bytes_read, 0); // Should be empty
		close(fd);
	}
	
	// Clean up
	unlink("test_file1.txt");
	unlink("test_file2.txt");
	unlink("test_file3.txt");
	
	gc_free_all(&gc);
}

Test(executor_tests, test_append_redirection_basic) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "hello", NULL};
	t_shell_context *ctx;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Create a command with append redirection
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->output_redirs = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_append.txt");
	
	// Execute the command
	cr_assert_eq(executor_execute(cmd_node, ctx), 0);
	
	// Check file exists and contains "hello"
	FILE *file = fopen("test_append.txt", "r");
	cr_assert_not_null(file);
	
	char buffer[256];
	cr_assert_not_null(fgets(buffer, sizeof(buffer), file));
	cr_assert_str_eq(buffer, "hello\n");
	
	fclose(file);
	unlink("test_append.txt");
	gc_free_all(&gc);
}

Test(executor_tests, test_append_redirection_existing_file) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "world", NULL};
	t_shell_context *ctx;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Create file with initial content
	FILE *file = fopen("test_append_existing.txt", "w");
	fprintf(file, "hello\n");
	fclose(file);
	
	// Create command with append redirection
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->output_redirs = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_append_existing.txt");
	
	// Execute the command
	cr_assert_eq(executor_execute(cmd_node, ctx), 0);
	
	// Check file contains both "hello" and "world"
	file = fopen("test_append_existing.txt", "r");
	cr_assert_not_null(file);
	
	char buffer[256];
	cr_assert_not_null(fgets(buffer, sizeof(buffer), file));
	cr_assert_str_eq(buffer, "hello\n");
	
	cr_assert_not_null(fgets(buffer, sizeof(buffer), file));
	cr_assert_str_eq(buffer, "world\n");
	
	fclose(file);
	unlink("test_append_existing.txt");
	gc_free_all(&gc);
}

Test(executor_tests, test_multiple_append_redirections) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "test", NULL};
	t_shell_context *ctx;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Create multiple append redirections
	t_redirection *redir1 = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_append1.txt");
	t_redirection *redir2 = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_append2.txt");
	t_redirection *redir3 = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_append3.txt");
	
	redir1->next = redir2;
	redir2->next = redir3;
	
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->output_redirs = redir1;
	
	// Execute the command
	cr_assert_eq(executor_execute(cmd_node, ctx), 0);
	
	// Check that all files were created
	cr_assert_eq(access("test_append1.txt", F_OK), 0);
	cr_assert_eq(access("test_append2.txt", F_OK), 0);
	cr_assert_eq(access("test_append3.txt", F_OK), 0);
	
	// Check that only the last file contains output
	FILE *file1 = fopen("test_append1.txt", "r");
	FILE *file2 = fopen("test_append2.txt", "r");
	FILE *file3 = fopen("test_append3.txt", "r");
	
	char buffer[256];
	
	// First two files should be empty
	cr_assert_null(fgets(buffer, sizeof(buffer), file1));
	cr_assert_null(fgets(buffer, sizeof(buffer), file2));
	
	// Last file should contain output
	cr_assert_not_null(fgets(buffer, sizeof(buffer), file3));
	cr_assert_str_eq(buffer, "test\n");
	
	fclose(file1);
	fclose(file2);
	fclose(file3);
	
	unlink("test_append1.txt");
	unlink("test_append2.txt");
	unlink("test_append3.txt");
	
	gc_free_all(&gc);
}

Test(executor_tests, test_mixed_truncate_append_redirections) {
	t_gc gc;
	t_ast_node *cmd_node;
	char *args[] = {"echo", "mixed", NULL};
	t_shell_context *ctx;
	
	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Create mixed redirections: > >> >
	t_redirection *redir1 = redirection_create(&gc, TOKEN_REDIR_OUT, "test_mixed1.txt");
	t_redirection *redir2 = redirection_create(&gc, TOKEN_REDIR_APPEND, "test_mixed2.txt");
	t_redirection *redir3 = redirection_create(&gc, TOKEN_REDIR_OUT, "test_mixed3.txt");
	
	redir1->next = redir2;
	redir2->next = redir3;
	
	cmd_node = ast_node_create(&gc, NODE_CMD);
	ast_node_set_args(&gc, cmd_node, args);
	cmd_node->output_redirs = redir1;
	
	// Execute the command
	cr_assert_eq(executor_execute(cmd_node, ctx), 0);
	
	// Check that all files were created
	cr_assert_eq(access("test_mixed1.txt", F_OK), 0);
	cr_assert_eq(access("test_mixed2.txt", F_OK), 0);
	cr_assert_eq(access("test_mixed3.txt", F_OK), 0);
	
	// Check that only the last file contains output
	FILE *file1 = fopen("test_mixed1.txt", "r");
	FILE *file2 = fopen("test_mixed2.txt", "r");
	FILE *file3 = fopen("test_mixed3.txt", "r");
	
	char buffer[256];
	
	// First two files should be empty
	cr_assert_null(fgets(buffer, sizeof(buffer), file1));
	cr_assert_null(fgets(buffer, sizeof(buffer), file2));
	
	// Last file should contain output (truncated)
	cr_assert_not_null(fgets(buffer, sizeof(buffer), file3));
	cr_assert_str_eq(buffer, "mixed\n");
	
	fclose(file1);
	fclose(file2);
	fclose(file3);
	
	unlink("test_mixed1.txt");
	unlink("test_mixed2.txt");
	unlink("test_mixed3.txt");
	
	gc_free_all(&gc);
}