#include <criterion/criterion.h>
#include "minishell.h"

Test(env_tests, test_env_create_and_basic_operations) {
	t_gc		gc;
	t_shell_env	*env;
	
	gc_init(&gc);
	env = env_create(&gc);
	
	cr_assert_not_null(env);
	cr_assert_null(env->vars);
	cr_assert_eq(env->last_exit_code, 0);
	cr_assert_null(env->cwd);
	
	gc_free_all(&gc);
}

Test(env_tests, test_env_set_and_get) {
	t_gc		gc;
	t_shell_env	*env;
	char		*value;
	
	gc_init(&gc);
	env = env_create(&gc);
	
	cr_assert_eq(env_set_var(&gc, env, "HOME", "/home/test"), 0);
	cr_assert_eq(env_set_var(&gc, env, "USER", "testuser"), 0);
	
	value = env_get_value(env, "HOME");
	cr_assert_not_null(value);
	cr_assert_str_eq(value, "/home/test");
	
	value = env_get_value(env, "USER");
	cr_assert_not_null(value);
	cr_assert_str_eq(value, "testuser");
	
	value = env_get_value(env, "NONEXISTENT");
	cr_assert_null(value);
	
	gc_free_all(&gc);
}

Test(env_tests, test_env_update_variable) {
	t_gc		gc;
	t_shell_env	*env;
	char		*value;
	
	gc_init(&gc);
	env = env_create(&gc);
	
	cr_assert_eq(env_set_var(&gc, env, "PATH", "/bin"), 0);
	value = env_get_value(env, "PATH");
	cr_assert_str_eq(value, "/bin");
	
	cr_assert_eq(env_set_var(&gc, env, "PATH", "/usr/bin:/bin"), 0);
	value = env_get_value(env, "PATH");
	cr_assert_str_eq(value, "/usr/bin:/bin");
	
	gc_free_all(&gc);
}

Test(env_tests, test_env_unset_variable) {
	t_gc		gc;
	t_shell_env	*env;
	
	gc_init(&gc);
	env = env_create(&gc);
	
	cr_assert_eq(env_set_var(&gc, env, "TEMP", "temporary"), 0);
	cr_assert_not_null(env_get_value(env, "TEMP"));
	
	cr_assert_eq(env_unset_var(env, "TEMP"), 0);
	cr_assert_null(env_get_value(env, "TEMP"));
	
	cr_assert_eq(env_unset_var(env, "NONEXISTENT"), -1);
	
	gc_free_all(&gc);
}

Test(env_tests, test_env_valid_name_validation) {
	cr_assert_eq(env_is_valid_name("HOME"), 1);
	cr_assert_eq(env_is_valid_name("_PRIVATE"), 1);
	cr_assert_eq(env_is_valid_name("VAR123"), 1);
	cr_assert_eq(env_is_valid_name("PATH_VAR"), 1);
	
	cr_assert_eq(env_is_valid_name("123VAR"), 0);
	cr_assert_eq(env_is_valid_name(""), 0);
	cr_assert_eq(env_is_valid_name(NULL), 0);
	cr_assert_eq(env_is_valid_name("VAR-NAME"), 0);
	cr_assert_eq(env_is_valid_name("VAR.NAME"), 0);
}

Test(env_tests, test_env_exit_code_tracking) {
	t_gc		gc;
	t_shell_env	*env;
	
	gc_init(&gc);
	env = env_create(&gc);
	
	cr_assert_eq(env->last_exit_code, 0);
	
	env_set_exit_code(env, 127);
	cr_assert_eq(env->last_exit_code, 127);
	
	env_set_exit_code(env, 0);
	cr_assert_eq(env->last_exit_code, 0);
	
	gc_free_all(&gc);
}
