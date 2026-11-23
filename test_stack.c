#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "stack.h"

static int total_tests = 0;
static int passed_tests = 0;

#define TEST_ASSERT(cond, msg) do { \
    total_tests++; \
    if (cond) { \
        passed_tests++; \
    } else { \
        printf("  [FAIL] %s (Line %d): %s\n", __func__, __LINE__, msg); \
        return; \
    } \
} while(0)

static void test_empty_stack_and_underflow(void) {
    ActionStack s;
    char out[ACTION_TEXT] = {0};
    stack_init(&s);

    TEST_ASSERT(s.top == -1, "New stack top should be -1");
    TEST_ASSERT(stack_pop(&s, out) == 0, "POP on empty stack must return 0 (underflow)");
    TEST_ASSERT(stack_peek(&s, out) == 0, "PEEK on empty stack must return 0");
    printf("  [PASS] test_empty_stack_and_underflow\n");
}

static void test_push_and_peek(void) {
    ActionStack s;
    char out[ACTION_TEXT] = {0};
    stack_init(&s);

    TEST_ASSERT(stack_push(&s, "Action One") == 1, "First push should succeed");
    TEST_ASSERT(s.top == 0, "Top should be 0 after 1 push");

    TEST_ASSERT(stack_peek(&s, out) == 1, "Peek should succeed");
    TEST_ASSERT(strcmp(out, "Action One") == 0, "Peeked text must match 'Action One'");
    TEST_ASSERT(s.top == 0, "Peek must not modify stack top");

    TEST_ASSERT(stack_push(&s, "Action Two") == 1, "Second push should succeed");
    TEST_ASSERT(s.top == 1, "Top should be 1 after 2 pushes");

    TEST_ASSERT(stack_peek(&s, out) == 1, "Peek should return 1");
    TEST_ASSERT(strcmp(out, "Action Two") == 0, "Peeked text must match 'Action Two'");
    TEST_ASSERT(s.top == 1, "Peek must still not modify stack top");
    printf("  [PASS] test_push_and_peek\n");
}

static void test_pop_lifo(void) {
    ActionStack s;
    char out[ACTION_TEXT] = {0};
    stack_init(&s);

    stack_push(&s, "First");
    stack_push(&s, "Second");
    stack_push(&s, "Third");

    TEST_ASSERT(stack_pop(&s, out) == 1, "First POP should succeed");
    TEST_ASSERT(strcmp(out, "Third") == 0, "LIFO: First pop must be 'Third'");

    TEST_ASSERT(stack_pop(&s, out) == 1, "Second POP should succeed");
    TEST_ASSERT(strcmp(out, "Second") == 0, "LIFO: Second pop must be 'Second'");

    TEST_ASSERT(stack_pop(&s, out) == 1, "Third POP should succeed");
    TEST_ASSERT(strcmp(out, "First") == 0, "LIFO: Third pop must be 'First'");

    TEST_ASSERT(stack_pop(&s, out) == 0, "Fourth POP should fail (underflow)");
    TEST_ASSERT(s.top == -1, "Stack top should be -1 after all items popped");
    printf("  [PASS] test_pop_lifo (Last-In, First-Out order)\n");
}

static void test_overflow(void) {
    ActionStack s;
    char buffer[ACTION_TEXT];
    stack_init(&s);

    for (int i = 0; i < MAX_ACTIONS; i++) {
        snprintf(buffer, sizeof(buffer), "Action #%d", i + 1);
        int res = stack_push(&s, buffer);
        TEST_ASSERT(res == 1, "Push up to MAX_ACTIONS must succeed");
    }

    TEST_ASSERT(s.top == MAX_ACTIONS - 1, "Stack top should be at MAX_ACTIONS - 1");

    /* 101st push must fail */
    int overflow_res = stack_push(&s, "Overflow Action");
    TEST_ASSERT(overflow_res == 0, "Pushing beyond MAX_ACTIONS must return 0");
    TEST_ASSERT(s.top == MAX_ACTIONS - 1, "Stack top must not change on overflow");

    /* Verify top item is still Action #100 */
    char out[ACTION_TEXT];
    TEST_ASSERT(stack_peek(&s, out) == 1, "Peek must succeed");
    TEST_ASSERT(strcmp(out, "Action #100") == 0, "Top item must remain Action #100");

    /* Pop all items back down */
    int popped = 0;
    while (stack_pop(&s, out)) {
        popped++;
    }
    TEST_ASSERT(popped == MAX_ACTIONS, "Should pop exactly MAX_ACTIONS items");
    TEST_ASSERT(s.top == -1, "Stack should be empty");
    printf("  [PASS] test_overflow and full drain\n");
}

int main(void) {
    printf("=========================================\n");
    printf("       RUNNING STACK UNIT TESTS\n");
    printf("=========================================\n");

    test_empty_stack_and_underflow();
    test_push_and_peek();
    test_pop_lifo();
    test_overflow();

    printf("-----------------------------------------\n");
    printf("Results: %d/%d tests passed\n", passed_tests, total_tests);
    printf("=========================================\n\n");

    return (passed_tests == total_tests) ? 0 : 1;
}
