# Quick C-based router
NAME = router
DFLAGS = -g -O0 -fsanitize=address -fsanitize-undefined-trap-on-error -DDEBUG_H
CFLAGS = -Wall -Werror -std=c99 -Ivendor
CC = clang


# main - Compile tests for files in tests/
main:
	$(CC) $(CFLAGS) -o bin/router vendor/zwalker.c router.c router-test.c

# debug - 
debug: CFLAGS += $(DFLAGS)
debug: $(TARGET) 
	$(CC) $(CFLAGS) -o bin/router vendor/zwalker.c router.c router-test.c

# test-debug - Compile tests for files in tests/
test-debug:
	$(CC) $(DFLAGS) -Wall -Werror -Ivendor -o bin/router vendor/zwalker.c vendor/router.c tests/router-test.c
