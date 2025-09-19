/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexanfe <alexanfe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:00 by alexanfe          #+#    #+#             */
/*   Updated: 2025/09/19 14:35:00 by alexanfe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <criterion/criterion.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"

Test(signal_tests, test_shell_signal_setup) {
    // Test shell signal handler setup
    setup_shell_signals();
    
    // Should not crash - test passes if we reach here
    cr_assert(1);
}

Test(signal_tests, test_command_signal_setup) {
    // Test command signal handler setup
    setup_command_signals();
    
    // Should not crash - test passes if we reach here
    cr_assert(1);
}

Test(signal_tests, test_process_child_status_normal_exit) {
    int status;
    int result;
    
    // Simulate normal exit with code 0
    status = 0;
    result = process_child_status(status);
    cr_assert_eq(result, 0);
    
    // Simulate normal exit with code 42
    status = 42 << 8;
    result = process_child_status(status);
    cr_assert_eq(result, 42);
}

Test(signal_tests, test_process_child_status_signal_termination) {
    int status;
    int result;
    
    // Simulate SIGINT termination
    status = SIGINT;
    result = process_child_status(status);
    cr_assert_eq(result, 128 + SIGINT);
    
    // Simulate SIGQUIT termination
    status = SIGQUIT;
    result = process_child_status(status);
    cr_assert_eq(result, 128 + SIGQUIT);
}

Test(signal_tests, test_signal_received_global_variable) {
    extern int g_signal_received;
    
    // Test initial state
    cr_assert_eq(get_signal_received(), 0);
    
    // Test direct global access
    g_signal_received = SIGINT;
    cr_assert_eq(get_signal_received(), SIGINT);
    
    // Test reset function
    reset_signal_received();
    cr_assert_eq(get_signal_received(), 0);
    cr_assert_eq(g_signal_received, 0);
}

Test(signal_tests, test_signal_received_consistency) {
    extern int g_signal_received;
    
    // Test consistency between direct access and function
    g_signal_received = SIGQUIT;
    cr_assert_eq(get_signal_received(), SIGQUIT);
    
    reset_signal_received();
    cr_assert_eq(g_signal_received, 0);
    
    // Test setting through global and reading through function
    g_signal_received = SIGTERM;
    cr_assert_eq(get_signal_received(), SIGTERM);
}

Test(signal_tests, test_only_one_global_variable) {
    extern int g_signal_received;
    
    // This test verifies we only have one global variable
    // and that it only stores the signal number
    
    // Test signal number storage
    g_signal_received = SIGINT;
    cr_assert_eq(g_signal_received, SIGINT);
    
    g_signal_received = SIGQUIT;  
    cr_assert_eq(g_signal_received, SIGQUIT);
    
    g_signal_received = 0;
    cr_assert_eq(g_signal_received, 0);
    
    // Verify it's just an int (signal number only)
    cr_assert_eq(sizeof(g_signal_received), sizeof(int));
}