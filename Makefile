# Quick C-based router
NAME = router
DFLAGS = -g -O0 -fsanitize=address -fsanitize-undefined-trap-on-error -DDEBUG_H
CFLAGS = -Wall -Werror -Wno-unused -Wno-format-security -fPIC -std=c99 -Ivendor
SFLAGS = -Wall -Werror -Wno-unused -Wno-format-security -std=c99 -Ivendor
CC = clang
SRC = vendor/zhttp.c vendor/zmime.c vendor/zwalker.c vendor/database.c \
	vendor/ztable.c vendor/zrender.c vendor/router.c vendor/megadeth.c
OBJ = $(SRC:.c=.o)
TARGET =


# main - Compile tests for files in tests/
main:
	$(CC) -Wall -Werror -Ivendor -o bin/router vendor/zwalker.c router.c router-test.c

# debug - 
debug: CFLAGS += $(DFLAGS)
debug: SFLAGS += $(DFLAGS)
debug: $(TARGET) 
	@test ! -z "$(TARGET)" || echo "TARGET env var not specified" > /dev/stderr
	@printf '' > /dev/null

# test-debug - Compile tests for files in tests/
test-debug:
	$(CC) $(DFLAGS) -Wall -Werror -Ivendor -o bin/router vendor/zwalker.c vendor/router.c tests/router-test.c
