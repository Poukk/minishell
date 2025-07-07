#include <criterion/criterion.h>
#include "minishell.h"

Test(sum_tests, test_positive_numbers) {
    int result = sum(5, 3);
    cr_assert_eq(result, 8, "Expected sum(5, 3) to equal 8, but got %d", result);
}

Test(sum_tests, test_negative_numbers) {
    int result = sum(-10, -5);
    cr_assert_eq(result, -15, "Expected sum(-10, -5) to equal -15, but got %d", result);
}

Test(sum_tests, test_mixed_numbers) {
    int result = sum(-7, 12);
    cr_assert_eq(result, 5, "Expected sum(-7, 12) to equal 5, but got %d", result);
}
