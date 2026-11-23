#include <stdio.h>
#include <string.h>
#include "stack.h"

void stack_init(ActionStack *s) { s->top = -1; }

int stack_push(ActionStack *s, const char *action) {
    if (s->top >= MAX_ACTIONS - 1) return 0;
    s->top++;
    snprintf(s->items[s->top], ACTION_TEXT, "%s", action);
    return 1;
}

int stack_pop(ActionStack *s, char *out) {
    if (s->top < 0) return 0;
    snprintf(out, ACTION_TEXT, "%s", s->items[s->top]);
    s->top--;
    return 1;
}

int stack_peek(const ActionStack *s, char *out) {
    if (s->top < 0) return 0;
    snprintf(out, ACTION_TEXT, "%s", s->items[s->top]);
    return 1;
}

void stack_display(const ActionStack *s) {
    if (s->top < 0) {
        printf("\nAction history is empty.\n");
        return;
    }
    printf("\nRecent actions (TOP to BOTTOM):\n");
    for (int i = s->top; i >= 0; i--) printf("%d. %s\n", s->top - i + 1, s->items[i]);
}
