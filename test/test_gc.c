#include <criterion/criterion.h>
#include "minishell.h"

Test(gc_tests, test_gc_init) {
    t_gc gc;
    
    gc_init(&gc);
    
    cr_assert_null(gc.allocated_ptrs, "Expected allocated_ptrs to be NULL after initialization");
}

Test(gc_tests, test_gc_malloc_single) {
    t_gc gc;
    void *ptr;
    
    gc_init(&gc);
    ptr = gc_malloc(&gc, 100);
    
    cr_assert_not_null(ptr, "Expected gc_malloc to return a valid pointer");
    cr_assert_not_null(gc.allocated_ptrs, "Expected allocated_ptrs list to be created");
    cr_assert_eq(gc.allocated_ptrs->content, ptr, "Expected list to contain the allocated pointer");
    
    gc_free_all(&gc);
}

Test(gc_tests, test_gc_malloc_multiple) {
    t_gc gc;
    void *ptr1, *ptr2, *ptr3;
    
    gc_init(&gc);
    ptr1 = gc_malloc(&gc, 50);
    ptr2 = gc_malloc(&gc, 100);
    ptr3 = gc_malloc(&gc, 200);
    
    cr_assert_not_null(ptr1, "Expected first gc_malloc to return a valid pointer");
    cr_assert_not_null(ptr2, "Expected second gc_malloc to return a valid pointer");
    cr_assert_not_null(ptr3, "Expected third gc_malloc to return a valid pointer");
    
    cr_assert_not_null(gc.allocated_ptrs, "Expected allocated_ptrs list to exist");
    
    int count = 0;
    t_list *current = gc.allocated_ptrs;
    while (current)
    {
        count++;
        current = current->next;
    }
    cr_assert_eq(count, 3, "Expected 3 allocations to be tracked, got %d", count);
    
    gc_free_all(&gc);
}

Test(gc_tests, test_gc_malloc_zero_size) {
    t_gc gc;
    void *ptr;
    
    gc_init(&gc);
    ptr = gc_malloc(&gc, 0);
    
    cr_assert_not_null(ptr, "Expected gc_malloc(0) to return a valid pointer");
    
    gc_free_all(&gc);
}

Test(gc_tests, test_gc_free_all_empty) {
    t_gc gc;
    
    gc_init(&gc);
    gc_free_all(&gc);
    
    cr_assert_null(gc.allocated_ptrs, "Expected allocated_ptrs to be NULL after gc_free_all on empty list");
}

Test(gc_tests, test_gc_free_all_with_allocations) {
    t_gc gc;
    void *ptr1, *ptr2;
    
    gc_init(&gc);
    ptr1 = gc_malloc(&gc, 50);
    ptr2 = gc_malloc(&gc, 100);
    
    cr_assert_not_null(ptr1, "Expected ptr1 to be allocated");
    cr_assert_not_null(ptr2, "Expected ptr2 to be allocated");
    cr_assert_not_null(gc.allocated_ptrs, "Expected allocated_ptrs to exist before gc_free_all");
    
    gc_free_all(&gc);
    
    cr_assert_null(gc.allocated_ptrs, "Expected allocated_ptrs to be NULL after gc_free_all");
}

Test(gc_tests, test_gc_multiple_free_all) {
    t_gc gc;
    void *ptr;
    
    gc_init(&gc);
    ptr = gc_malloc(&gc, 50);
    
    cr_assert_not_null(ptr, "Expected ptr to be allocated");
    
    gc_free_all(&gc);
    cr_assert_null(gc.allocated_ptrs, "Expected allocated_ptrs to be NULL after first gc_free_all");
    
    gc_free_all(&gc);
    cr_assert_null(gc.allocated_ptrs, "Expected allocated_ptrs to remain NULL after second gc_free_all");
}