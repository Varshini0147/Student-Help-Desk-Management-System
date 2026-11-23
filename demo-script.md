# Demo recording script (about 3–4 minutes)

## Opening
“Hello, we are presenting our Student Help Desk Management System. It is a C command-line application that demonstrates two data structures: a priority queue and a stack.”

## Explain the problem
“Student service requests can be difficult to organize informally. Our system records requests and processes urgent requests before routine requests.”

## Explain the priority queue
“Our priority levels are High, Medium and Low. High has the highest priority. If two requests have the same priority, the earlier request is processed first. This is the FIFO tie-break rule.”

## Live demonstration
1. Compile and run the program.
2. Choose option 1 and add a Low-priority request.
3. Choose option 1 and add a High-priority request.
4. Add a second High-priority request.
5. Choose option 2. Explain that High requests are shown before Low and that the two High requests retain arrival order.
6. Choose option 3. Explain that the first High request is processed.

## Explain the stack
“Every successful submission and processing action is pushed onto the stack. The latest action is shown first because a stack follows LIFO: Last In, First Out.”
7. Choose option 4 to display history.
8. Choose option 5 to peek at the latest action.
9. Choose option 6 to POP the latest action.

## Boundary handling
“An empty queue and an empty stack are handled safely. Invalid menu choices are also rejected with a message.”
10. If convenient, try Process Next after the queue becomes empty, or POP until the stack is empty.

## Closing
“To conclude, the priority queue manages pending requests by urgency, while the stack maintains recent-action history. Thank you.”
