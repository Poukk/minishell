/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:45:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 14:45:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "minishell.h"

Test(error_tests, test_error_constants) {
    // Test that our error constants are correct
    cr_assert_eq(EXIT_SUCCESS, 0);
    cr_assert_eq(EXIT_GENERAL_ERROR, 1);
    cr_assert_eq(EXIT_MISUSE, 2);
    cr_assert_eq(EXIT_EXEC_FAILED, 126);
    cr_assert_eq(EXIT_CMD_NOT_FOUND, 127);
}

Test(error_tests, test_return_error_code_with_message, .init = cr_redirect_stderr) {
    int result;
    
    // Test with command and message
    result = return_error_code(EXIT_CMD_NOT_FOUND, "test_cmd", NULL, "command not found");
    cr_assert_eq(result, EXIT_CMD_NOT_FOUND);
    
    // Test with command, arg and message
    result = return_error_code(EXIT_EXEC_FAILED, "test_cmd", "test_arg", "Is a directory");
    cr_assert_eq(result, EXIT_EXEC_FAILED);
}

Test(error_tests, test_return_error_code_without_message) {
    int result;
    
    // Test without message (should not print anything)
    result = return_error_code(EXIT_GENERAL_ERROR, "test_cmd", NULL, NULL);
    cr_assert_eq(result, EXIT_GENERAL_ERROR);
}

Test(error_tests, test_print_error_functions, .init = cr_redirect_stderr) {
    // Test print_command_error
    print_command_error("test_cmd", "test message");
    
    // Test print_error with all parameters
    print_error("test_cmd", "test_arg", "test message");
    
    // Test print_error with minimal parameters
    print_error(NULL, NULL, "test message");
}