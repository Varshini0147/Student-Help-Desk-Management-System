#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"
#include "priority_queue.h"
#include "stack.h"

static int get_int(const char *prompt, int *out) {
    char line[40], *end;
    long value;
    while (1) {
        printf("%s", prompt);
        if (!fgets(line, sizeof line, stdin)) return 0; /* EOF encountered */
        if (strchr(line, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        value = strtol(line, &end, 10);
        while (*end == ' ' || *end == '\t') end++;
        if (end != line && (*end == '\n' || *end == '\0')) {
            *out = (int)value;
            return 1;
        }
        printf("Invalid input. Please enter a number.\n");
    }
}

static void submit_request(PriorityQueue *q, ActionStack *s, int *next_id) {
    Request r = {0};
    int p;
    if (q->size >= MAX_REQUESTS) {
        printf("\nQueue is full. Cannot accept more requests.\n");
        return;
    }

    printf("Student name: ");
    if (!read_line(r.student, sizeof r.student)) {
        printf("\nInput ended. Request cancelled.\n");
        return;
    }

    printf("Category (ID card/Lab access/Fees/Scholarship/General): ");
    if (!read_line(r.category, sizeof r.category)) {
        printf("\nInput ended. Request cancelled.\n");
        return;
    }

    printf("Short description: ");
    if (!read_line(r.description, sizeof r.description)) {
        printf("\nInput ended. Request cancelled.\n");
        return;
    }

    do {
        if (!get_int("Priority (1=Low, 2=Medium, 3=High): ", &p)) {
            printf("\nInput ended. Request cancelled.\n");
            return;
        }
        if (p < 1 || p > 3) printf("Choose 1, 2 or 3.\n");
    } while (p < 1 || p > 3);

    r.id = (*next_id)++;
    r.priority = (Priority)p;

    if (pq_insert(q, r)) {
        char action[ACTION_TEXT];
        snprintf(action, sizeof action, "Submitted request #%d (%s priority)", r.id, priority_name(r.priority));
        stack_push(s, action);
        printf("\nRequest submitted successfully. Your Request ID is #%d.\n", r.id);
    } else {
        printf("\nQueue is full. Request was not added.\n");
    }
}

static void process_next(PriorityQueue *q, ActionStack *s) {
    Request r;
    if (!pq_process_next(q, &r)) {
        printf("\nNo pending requests to process.\n");
        return;
    }
    char action[ACTION_TEXT];
    snprintf(action, sizeof action, "Processed request #%d for %s", r.id, r.student);
    stack_push(s, action);
    printf("\nProcessing request #%d\nStudent: %s\nCategory: %s\nPriority: %s\n",
           r.id, r.student, r.category, priority_name(r.priority));
}

int main(void) {
    PriorityQueue queue;
    ActionStack history;
    int next_id = 101, choice;
    pq_init(&queue);
    stack_init(&history);

    while (1) {
        printf("\n==============================================\n");
        printf("       STUDENT HELP DESK MANAGEMENT SYSTEM\n");
        printf("==============================================\n");
        printf("1. Submit student request\n");
        printf("2. Display pending requests\n");
        printf("3. Process next request\n");
        printf("4. Display recent-action history\n");
        printf("5. Peek latest action\n");
        printf("6. POP latest action\n");
        printf("0. Exit\n");
        printf("----------------------------------------------\n");

        if (!get_int("Enter your choice: ", &choice)) {
            printf("\nEnd of input detected. Exiting.\n");
            break;
        }
        if (choice == 0) {
            printf("\nThank you for using the Student Help Desk.\n");
            break;
        }

        switch (choice) {
            case 1: submit_request(&queue, &history, &next_id); break;
            case 2: pq_display(&queue); break;
            case 3: process_next(&queue, &history); break;
            case 4: stack_display(&history); break;
            case 5: {
                char action[ACTION_TEXT];
                if (stack_peek(&history, action)) {
                    printf("\nLatest action: %s\n", action);
                } else {
                    printf("\nAction history is empty.\n");
                }
                break;
            }
            case 6: {
                char action[ACTION_TEXT];
                if (stack_pop(&history, action)) {
                    printf("\nPopped action: %s\n", action);
                } else {
                    printf("\nStack underflow: no action to remove.\n");
                }
                break;
            }
            default: printf("\nInvalid menu choice. Please try again.\n");
        }
    }
    return 0;
}
