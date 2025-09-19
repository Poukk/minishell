#include <criterion/criterion.h>
#include "minishell.h"

static t_shell_context *create_test_context(t_gc *gc) {
	t_shell_context *ctx;
	
	ctx = gc_malloc(gc, sizeof(t_shell_context));
	if (!ctx)
		return (NULL);
	ctx->env = env_create(gc);
	ctx->gc = gc;
	return (ctx);
}

Test(builtin_tests, test_is_builtin_command_echo) {
	t_builtin_type	result;

	result = is_builtin_command("echo");
	cr_assert_eq(result, BUILTIN_ECHO);
}

Test(builtin_tests, test_is_builtin_command_cd) {
	t_builtin_type	result;

	result = is_builtin_command("cd");
	cr_assert_eq(result, BUILTIN_CD);
}

Test(builtin_tests, test_is_builtin_command_pwd) {
	t_builtin_type	result;

	result = is_builtin_command("pwd");
	cr_assert_eq(result, BUILTIN_PWD);
}

Test(builtin_tests, test_is_builtin_command_env) {
	t_builtin_type	result;

	result = is_builtin_command("env");
	cr_assert_eq(result, BUILTIN_ENV);
}

Test(builtin_tests, test_is_builtin_command_export) {
	t_builtin_type	result;

	result = is_builtin_command("export");
	cr_assert_eq(result, BUILTIN_EXPORT);
}

Test(builtin_tests, test_is_builtin_command_unset) {
	t_builtin_type	result;

	result = is_builtin_command("unset");
	cr_assert_eq(result, BUILTIN_UNSET);
}

Test(builtin_tests, test_is_builtin_command_exit) {
	t_builtin_type	result;

	result = is_builtin_command("exit");
	cr_assert_eq(result, BUILTIN_EXIT);
}

Test(builtin_tests, test_is_builtin_command_none) {
	t_builtin_type	result;

	result = is_builtin_command("ls");
	cr_assert_eq(result, BUILTIN_NONE);
}

Test(builtin_tests, test_is_builtin_command_null) {
	t_builtin_type	result;

	result = is_builtin_command(NULL);
	cr_assert_eq(result, BUILTIN_NONE);
}

Test(builtin_tests, test_is_builtin_command_empty) {
	t_builtin_type	result;

	result = is_builtin_command("");
	cr_assert_eq(result, BUILTIN_NONE);
}

Test(builtin_tests, test_execute_builtin_echo) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"echo", "hello", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_invalid) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"echo", "hello", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_NONE, args, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_null_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, NULL, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_pwd_basic) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"pwd", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_PWD, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_pwd_with_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"pwd", "extra", "args", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_PWD, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_pwd_integration) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"pwd", NULL};
	int				result;
	extern char		**environ;

	gc_init(&gc);
	ctx = gc_malloc(&gc, sizeof(t_shell_context));
	ctx->gc = &gc;
	ctx->env = env_init_from_system(&gc, environ);
	result = execute_builtin(BUILTIN_PWD, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_pwd_output_format) {
	t_gc			gc;
	int				result;

	gc_init(&gc);
	result = builtin_pwd();
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_env_basic) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"env", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "TEST_VAR", "test_value");
	result = execute_builtin(BUILTIN_ENV, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_env_with_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"env", "extra", "args", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ENV, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_env_integration) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"env", NULL};
	int				result;
	extern char		**environ;

	gc_init(&gc);
	ctx = gc_malloc(&gc, sizeof(t_shell_context));
	ctx->gc = &gc;
	ctx->env = env_init_from_system(&gc, environ);
	result = execute_builtin(BUILTIN_ENV, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_env_direct_call) {
	t_gc			gc;
	t_shell_context	*ctx;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "HOME", "/home/user");
	env_set_var(&gc, ctx->env, "PATH", "/usr/bin:/bin");
	result = builtin_env(ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_env_empty_environment) {
	t_gc			gc;
	t_shell_context	*ctx;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = builtin_env(ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_env_null_environment) {
	int				result;

	result = builtin_env(NULL);
	cr_assert_eq(result, 1);
}

Test(builtin_tests, test_env_with_special_characters) {
	t_gc			gc;
	t_shell_context	*ctx;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "TEST_VAR", "value with spaces and symbols !@#$%");
	env_set_var(&gc, ctx->env, "EMPTY_VAR", "");
	result = builtin_env(ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_cd_basic) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", "/tmp", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "HOME", "/home/user");
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_geq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_cd_home) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "HOME", "/home/user");
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_geq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_cd_nonexistent) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", "/nonexistent/directory/path", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_cd_no_home) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_cd_too_many_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", "/tmp", "extra", "args", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_current_directory) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", ".", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_null_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_CD, NULL, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_null_env) {
	t_gc			gc;
	char			*args[] = {"cd", "/tmp", NULL};
	int				result;

	gc_init(&gc);
	result = execute_builtin(BUILTIN_CD, args, NULL);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_pwd_update) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"cd", ".", NULL};
	char			*pwd_after;
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "PWD", "/initial/path");
	result = execute_builtin(BUILTIN_CD, args, ctx);
	pwd_after = env_get_value(ctx->env, "PWD");
	cr_assert_eq(result, 0);
	cr_assert_not_null(pwd_after);
	cr_assert_str_neq(pwd_after, "/initial/path");
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_basic) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "TEST_VAR=test_value", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	value = env_get_value(ctx->env, "TEST_VAR");
	cr_assert_eq(result, 0);
	cr_assert_not_null(value);
	cr_assert_str_eq(value, "test_value");
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_multiple) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "VAR1=value1", "VAR2=value2", NULL};
	int				result;
	char			*value1;
	char			*value2;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	value1 = env_get_value(ctx->env, "VAR1");
	value2 = env_get_value(ctx->env, "VAR2");
	cr_assert_eq(result, 0);
	cr_assert_not_null(value1);
	cr_assert_not_null(value2);
	cr_assert_str_eq(value1, "value1");
	cr_assert_str_eq(value2, "value2");
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_no_args) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "TEST_VAR", "test_value");
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_invalid_name) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "123INVALID=value", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	cr_assert_eq(result, 1);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_no_value) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "VALID_VAR", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_execute_builtin_export_empty_value) {
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "EMPTY_VAR=", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_EXPORT, args, ctx);
	value = env_get_value(ctx->env, "EMPTY_VAR");
	cr_assert_eq(result, 0);
	cr_assert_not_null(value);
	cr_assert_str_eq(value, "");
	gc_free_all(&gc);
}
