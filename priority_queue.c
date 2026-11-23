#include <stdio.h>
#include "priority_queue.h"

void pq_init(PriorityQueue *q) { q->size = 0; }
int pq_is_empty(const PriorityQueue *q) { return q->size == 0; }

int pq_insert(PriorityQueue *q, Request r) {
    if (q->size >= MAX_REQUESTS) return 0;
    int i = q->size;
    /* Higher priority comes first. For equal priority, older arrival stays first. */
    while (i > 0 && q->items[i - 1].priority < r.priority) {
        q->items[i] = q->items[i - 1];
        i--;
    }
    q->items[i] = r;
    q->size++;
    return 1;
}

void pq_display(const PriorityQueue *q) {
    if (pq_is_empty(q)) {
        printf("\nNo pending requests.\n");
        return;
    }
    printf("\n%-6s %-18s %-14s %-8s %s\n", "ID", "STUDENT", "CATEGORY", "PRIORITY", "DESCRIPTION");
    printf("-------------------------------------------------------------------------------\n");
    for (int i = 0; i < q->size; i++) {
        const Request *r = &q->items[i];
        printf("%-6d %-18s %-14s %-8s %s\n", r->id, r->student, r->category,
               priority_name(r->priority), r->description);
    }
}

int pq_process_next(PriorityQueue *q, Request *out) {
    if (pq_is_empty(q)) return 0;
    *out = q->items[0];
    for (int i = 1; i < q->size; i++) q->items[i - 1] = q->items[i];
    q->size--;
    return 1;
}
