CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Iinclude

SRC = src/request.c src/priority_queue.c src/stack.c
MAIN_SRC = src/main.c $(SRC)
BIN = helpdesk

TEST_PQ_SRC = tests/test_priority_queue.c src/priority_queue.c src/request.c
TEST_PQ_BIN = test_priority_queue

TEST_STACK_SRC = tests/test_stack.c src/stack.c
TEST_STACK_BIN = test_stack

.PHONY: all clean test

all: $(BIN) $(TEST_PQ_BIN) $(TEST_STACK_BIN)

$(BIN): $(MAIN_SRC)
	$(CC) $(CFLAGS) $(MAIN_SRC) -o $(BIN)

$(TEST_PQ_BIN): $(TEST_PQ_SRC)
	$(CC) $(CFLAGS) $(TEST_PQ_SRC) -o $(TEST_PQ_BIN)

$(TEST_STACK_BIN): $(TEST_STACK_SRC)
	$(CC) $(CFLAGS) $(TEST_STACK_SRC) -o $(TEST_STACK_BIN)

test: $(TEST_PQ_BIN) $(TEST_STACK_BIN)
	@echo "--- Running Priority Queue Tests ---"
	./$(TEST_PQ_BIN)
	@echo "--- Running Stack Tests ---"
	./$(TEST_STACK_BIN)

clean:
	rm -f $(BIN) $(BIN).exe $(TEST_PQ_BIN) $(TEST_PQ_BIN).exe $(TEST_STACK_BIN) $(TEST_STACK_BIN).exe *.o *.obj
