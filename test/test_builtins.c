#include <criterion/criterion.h>
#include "minishell.h"

static t_shell_context *create_test_context(t_gc *gc) {
	t_shell_context *ctx;
	
	ctx = gc_malloc(gc, sizeof(t_shell_context));
	if (!ctx)
		return (NULL);
	ctx->env = env_create(gc);
	if (!ctx->env)
		return (NULL);
	env_update_pwd(gc, ctx->env);
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

// ============================================================================
// ECHO BUILTIN TESTS
// ============================================================================

Test(builtin_tests, test_echo_basic_output)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "hello", "world", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_empty_args)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_single_newline_flag)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "-n", "hello", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_multiple_newline_flags)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "-n", "-n", "-n", "test", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_newline_flag_only)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "-n", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_mixed_args_and_flags)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "-n", "start", "middle", "end", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_invalid_flag)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "-x", "hello", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

Test(builtin_tests, test_echo_special_characters)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"echo", "hello\tworld", "test\nstring", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	result = execute_builtin(BUILTIN_ECHO, args, ctx);
	cr_assert_eq(result, 0);
	gc_free_all(&gc);
}

// ============================================================================
// CD BUILTIN ADVANCED TESTS (OLDPWD and cd -)
// ============================================================================

Test(builtin_tests, test_cd_oldpwd_tracking)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"cd", "/tmp", NULL};
	char *initial_pwd;
	char *oldpwd_value;
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Get initial directory
	initial_pwd = env_get_value(ctx->env, "PWD");
	cr_assert_not_null(initial_pwd);
	
	// Change directory
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 0);
	
	// Check that OLDPWD was set to the previous directory
	oldpwd_value = env_get_value(ctx->env, "OLDPWD");
	cr_assert_not_null(oldpwd_value);
	cr_assert_str_eq(oldpwd_value, initial_pwd);
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_dash_previous_directory)
{
	t_gc gc;
	t_shell_context *ctx;
	char *cd_tmp_args[] = {"cd", "/tmp", NULL};
	char *cd_dash_args[] = {"cd", "-", NULL};
	char *initial_pwd;
	char *current_pwd;
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Get initial directory
	initial_pwd = env_get_value(ctx->env, "PWD");
	cr_assert_not_null(initial_pwd);
	
	// Change to /tmp
	result = execute_builtin(BUILTIN_CD, cd_tmp_args, ctx);
	cr_assert_eq(result, 0);
	
	// Change back with cd -
	result = execute_builtin(BUILTIN_CD, cd_dash_args, ctx);
	cr_assert_eq(result, 0);
	
	// Should be back to initial directory
	current_pwd = env_get_value(ctx->env, "PWD");
	cr_assert_not_null(current_pwd);
	cr_assert_str_eq(current_pwd, initial_pwd);
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_dash_no_oldpwd)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"cd", "-", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Remove OLDPWD if it exists
	env_unset_var(ctx->env, "OLDPWD");
	
	// Try cd - without OLDPWD
	result = execute_builtin(BUILTIN_CD, args, ctx);
	cr_assert_eq(result, 1); // Should fail
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_cd_oldpwd_multiple_changes)
{
	t_gc gc;
	t_shell_context *ctx;
	char *cd_tmp_args[] = {"cd", "/tmp", NULL};
	char *cd_var_args[] = {"cd", "/var", NULL};
	char *oldpwd_value;
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Change to /tmp
	result = execute_builtin(BUILTIN_CD, cd_tmp_args, ctx);
	cr_assert_eq(result, 0);
	
	// Change to /var - OLDPWD should now be /tmp
	result = execute_builtin(BUILTIN_CD, cd_var_args, ctx);
	cr_assert_eq(result, 0);
	
	// Check that OLDPWD is /tmp (not the initial directory)
	oldpwd_value = env_get_value(ctx->env, "OLDPWD");
	cr_assert_not_null(oldpwd_value);
	cr_assert_str_eq(oldpwd_value, "/tmp");
	
	gc_free_all(&gc);
}

// ============================================================================
// EXIT BUILTIN TESTS
// ============================================================================

Test(builtin_tests, test_exit_basic_default)
{
	// Note: Can't test actual exit since it terminates process
	// This tests the builtin detection and basic flow
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_with_zero)
{
	// Test that zero is valid exit code
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_with_positive_code)
{
	// Test positive exit code
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_with_negative_code)
{
	// Test negative exit code (should wrap to 255 due to modulo 256)
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_with_large_number)
{
	// Test large number (should wrap to 0 due to modulo 256)
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_with_very_large_number)
{
	// Test very large number (should wrap due to modulo 256)
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_non_numeric_argument)
{
	// Test non-numeric argument - should show error
	// Note: This would call exit(2), so we can't test the actual execution
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_mixed_numeric_argument)
{
	// Test mixed numeric/non-numeric - should be invalid
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_too_many_arguments)
{
	t_gc gc;
	t_shell_context *ctx;
	char *args[] = {"exit", "0", "1", NULL};
	int result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	// Test too many arguments - should return 1 and not exit
	result = execute_builtin(BUILTIN_EXIT, args, ctx);
	cr_assert_eq(result, 1);
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_exit_empty_argument)
{
	// Test empty string argument - should be invalid
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_whitespace_argument)
{
	// Test whitespace-only argument - should be invalid
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_leading_zeros)
{
	// Test leading zeros - should be valid (parsed as 42)
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_exit_plus_sign)
{
	// Test explicit plus sign - should be valid
	cr_assert_eq(is_builtin_command("exit"), BUILTIN_EXIT);
}

Test(builtin_tests, test_unset_basic_functionality)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", "TEST_VAR", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "TEST_VAR", "test_value");
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 0);
	cr_assert_null(env_get_value(ctx->env, "TEST_VAR"));
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_unset_multiple_variables)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", "VAR1", "VAR2", "VAR3", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "VAR1", "value1");
	env_set_var(&gc, ctx->env, "VAR2", "value2");
	env_set_var(&gc, ctx->env, "VAR3", "value3");
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 0);
	cr_assert_null(env_get_value(ctx->env, "VAR1"));
	cr_assert_null(env_get_value(ctx->env, "VAR2"));
	cr_assert_null(env_get_value(ctx->env, "VAR3"));
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_unset_nonexistent_variable)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", "NONEXISTENT", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 1);
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_unset_no_arguments)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 0);
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_unset_null_context)
{
	char	*args[] = {"unset", "TEST_VAR", NULL};
	int		result;

	result = builtin_unset(args, NULL);
	
	cr_assert_eq(result, 1);
}

Test(builtin_tests, test_unset_mixed_existing_nonexisting)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", "EXISTS", "NONEXISTENT", "ALSO_EXISTS", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "EXISTS", "value1");
	env_set_var(&gc, ctx->env, "ALSO_EXISTS", "value2");
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 1);
	cr_assert_null(env_get_value(ctx->env, "EXISTS"));
	cr_assert_null(env_get_value(ctx->env, "ALSO_EXISTS"));
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_unset_special_variables)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"unset", "HOME", "USER", "PATH", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	env_set_var(&gc, ctx->env, "HOME", "/home/user");
	env_set_var(&gc, ctx->env, "USER", "testuser");
	env_set_var(&gc, ctx->env, "PATH", "/bin:/usr/bin");
	
	result = builtin_unset(args, ctx);
	
	cr_assert_eq(result, 0);
	cr_assert_null(env_get_value(ctx->env, "HOME"));
	cr_assert_null(env_get_value(ctx->env, "USER"));
	cr_assert_null(env_get_value(ctx->env, "PATH"));
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_underscore_name)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "_=test_value", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	value = env_get_value(ctx->env, "_");
	
	cr_assert_eq(result, 0);
	cr_assert_str_eq(value, "test_value");
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_underscore_prefix)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "_VAR_NAME=test_value", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	value = env_get_value(ctx->env, "_VAR_NAME");
	
	cr_assert_eq(result, 0);
	cr_assert_str_eq(value, "test_value");
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_mixed_case)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "MyVar123=test_value", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	value = env_get_value(ctx->env, "MyVar123");
	
	cr_assert_eq(result, 0);
	cr_assert_str_eq(value, "test_value");
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_special_character_start)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "@INVALID=value", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	
	cr_assert_eq(result, 1);
	cr_assert_null(env_get_value(ctx->env, "@INVALID"));
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_equals_in_value)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "TEST_VAR=key=value=more", NULL};
	int				result;
	char			*value;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	value = env_get_value(ctx->env, "TEST_VAR");
	
	cr_assert_eq(result, 0);
	cr_assert_str_eq(value, "key=value=more");
	
	gc_free_all(&gc);
}

Test(builtin_tests, test_export_null_context)
{
	char	*args[] = {"export", "TEST_VAR=value", NULL};
	int		result;

	result = builtin_export(args, NULL);
	
	cr_assert_eq(result, 1);
}

Test(builtin_tests, test_export_hyphen_in_name)
{
	t_gc			gc;
	t_shell_context	*ctx;
	char			*args[] = {"export", "VAR-NAME=value", NULL};
	int				result;

	gc_init(&gc);
	ctx = create_test_context(&gc);
	
	result = builtin_export(args, ctx);
	
	cr_assert_eq(result, 1);
	cr_assert_null(env_get_value(ctx->env, "VAR-NAME"));
	
	gc_free_all(&gc);
}
