#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "request.h"
#include "priority_queue.h"

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

static Request create_dummy_request(int id, const char *student, Priority p) {
    Request r = {0};
    r.id = id;
    snprintf(r.student, sizeof(r.student), "%s", student);
    snprintf(r.category, sizeof(r.category), "General");
    snprintf(r.description, sizeof(r.description), "Test request");
    r.priority = p;
    return r;
}

static void test_empty_queue(void) {
    PriorityQueue q;
    Request out;
    pq_init(&q);

    TEST_ASSERT(pq_is_empty(&q) == 1, "New queue should be empty");
    TEST_ASSERT(q.size == 0, "Queue size should be 0");
    TEST_ASSERT(pq_process_next(&q, &out) == 0, "Processing empty queue must return 0");
    printf("  [PASS] test_empty_queue\n");
}

static void test_priority_ordering(void) {
    PriorityQueue q;
    Request out;
    pq_init(&q);

    /* Insert in reverse priority order: Low, then Medium, then High */
    pq_insert(&q, create_dummy_request(1, "Alice", LOW));
    pq_insert(&q, create_dummy_request(2, "Bob", MEDIUM));
    pq_insert(&q, create_dummy_request(3, "Charlie", HIGH));

    TEST_ASSERT(q.size == 3, "Queue size should be 3");

    /* First processed must be HIGH */
    TEST_ASSERT(pq_process_next(&q, &out) == 1, "Process next should succeed");
    TEST_ASSERT(out.id == 3 && out.priority == HIGH, "First out must be High priority (#3 Charlie)");

    /* Second processed must be MEDIUM */
    TEST_ASSERT(pq_process_next(&q, &out) == 1, "Process next should succeed");
    TEST_ASSERT(out.id == 2 && out.priority == MEDIUM, "Second out must be Medium priority (#2 Bob)");

    /* Third processed must be LOW */
    TEST_ASSERT(pq_process_next(&q, &out) == 1, "Process next should succeed");
    TEST_ASSERT(out.id == 1 && out.priority == LOW, "Third out must be Low priority (#1 Alice)");

    TEST_ASSERT(pq_is_empty(&q) == 1, "Queue should now be empty");
    printf("  [PASS] test_priority_ordering (High > Medium > Low)\n");
}

static void test_fifo_tie_breaking(void) {
    PriorityQueue q;
    Request out;
    pq_init(&q);

    /* Add three High-priority requests in sequence */
    pq_insert(&q, create_dummy_request(101, "HighFirst", HIGH));
    pq_insert(&q, create_dummy_request(102, "HighSecond", HIGH));
    pq_insert(&q, create_dummy_request(103, "HighThird", HIGH));

    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 101, "First High request (101) must exit first");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 102, "Second High request (102) must exit second");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 103, "Third High request (103) must exit third");
    TEST_ASSERT(pq_is_empty(&q) == 1, "Queue should be empty after tie-break test");
    printf("  [PASS] test_fifo_tie_breaking (Equal priority strictly follows FIFO)\n");
}

static void test_interleaved_priority_fifo(void) {
    PriorityQueue q;
    Request out;
    pq_init(&q);

    /* Interleave arrivals of varying priorities */
    pq_insert(&q, create_dummy_request(1, "Low-1", LOW));
    pq_insert(&q, create_dummy_request(2, "High-1", HIGH));
    pq_insert(&q, create_dummy_request(3, "Med-1", MEDIUM));
    pq_insert(&q, create_dummy_request(4, "High-2", HIGH));
    pq_insert(&q, create_dummy_request(5, "Med-2", MEDIUM));

    /* Expected order: High-1 (2), High-2 (4), Med-1 (3), Med-2 (5), Low-1 (1) */
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 2, "Expected High-1 (#2)");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 4, "Expected High-2 (#4)");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 3, "Expected Med-1 (#3)");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 5, "Expected Med-2 (#5)");
    TEST_ASSERT(pq_process_next(&q, &out) == 1 && out.id == 1, "Expected Low-1 (#1)");
    TEST_ASSERT(pq_is_empty(&q) == 1, "Queue should be empty");
    printf("  [PASS] test_interleaved_priority_fifo\n");
}

static void test_full_queue_and_overflow(void) {
    PriorityQueue q;
    Request out;
    pq_init(&q);

    for (int i = 0; i < MAX_REQUESTS; i++) {
        Priority p = (i % 3 == 0) ? HIGH : (i % 3 == 1 ? MEDIUM : LOW);
        int res = pq_insert(&q, create_dummy_request(i + 1, "BulkStudent", p));
        TEST_ASSERT(res == 1, "Insertion up to MAX_REQUESTS must succeed");
    }

    TEST_ASSERT(q.size == MAX_REQUESTS, "Queue size should be exactly MAX_REQUESTS");

    /* Attempting 101st insertion must fail */
    int overflow_res = pq_insert(&q, create_dummy_request(999, "OverflowStudent", HIGH));
    TEST_ASSERT(overflow_res == 0, "Inserting beyond MAX_REQUESTS must return 0");
    TEST_ASSERT(q.size == MAX_REQUESTS, "Queue size must not exceed MAX_REQUESTS");

    /* Empty the entire queue */
    int dequeued = 0;
    while (pq_process_next(&q, &out)) {
        dequeued++;
    }
    TEST_ASSERT(dequeued == MAX_REQUESTS, "Should dequeue exactly MAX_REQUESTS items");
    TEST_ASSERT(pq_is_empty(&q) == 1, "Queue must be empty after full drain");
    printf("  [PASS] test_full_queue_and_overflow\n");
}

int main(void) {
    printf("=========================================\n");
    printf("  RUNNING PRIORITY QUEUE UNIT TESTS\n");
    printf("=========================================\n");

    test_empty_queue();
    test_priority_ordering();
    test_fifo_tie_breaking();
    test_interleaved_priority_fifo();
    test_full_queue_and_overflow();

    printf("-----------------------------------------\n");
    printf("Results: %d/%d tests passed\n", passed_tests, total_tests);
    printf("=========================================\n\n");

    return (passed_tests == total_tests) ? 0 : 1;
}
