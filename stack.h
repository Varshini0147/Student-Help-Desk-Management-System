#ifndef STACK_H
#define STACK_H
#define MAX_ACTIONS 100
#define ACTION_TEXT 160

typedef struct {
    char items[MAX_ACTIONS][ACTION_TEXT];
    int top;
} ActionStack;

void stack_init(ActionStack *s);
int stack_push(ActionStack *s, const char *action);
int stack_pop(ActionStack *s, char *out);
int stack_peek(const ActionStack *s, char *out);
void stack_display(const ActionStack *s);

#endif
