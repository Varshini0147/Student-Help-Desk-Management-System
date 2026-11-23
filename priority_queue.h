#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include "request.h"

typedef struct {
    Request items[MAX_REQUESTS];
    int size;
} PriorityQueue;

void pq_init(PriorityQueue *q);
int pq_insert(PriorityQueue *q, Request r);
int pq_is_empty(const PriorityQueue *q);
void pq_display(const PriorityQueue *q);
int pq_process_next(PriorityQueue *q, Request *out);

#endif
