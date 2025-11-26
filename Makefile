CC=gcc
CFLAGS=-I. -Icore/utils -Wall
LDFLAGS=-lws2_32

SRC= \
    core/utils/ui.c \
    core/memory/allocator.c \
    core/utils/logger.c \
    core/cli/cli.c \
    core/login/login.c \
    core/menu/menu.c \
    shell/shell.c \
    shell/parser.c \
    shell/executor.c \
    http/server.c \
    http/router.c \
    db/engine.c \
    db/storage.c \
    monitor/system.c \
    monitor/process.c \
    tools/hash.c \
    tools/compare.c \
    tools/compress.c \
    tools/decompress.c \
    tools/fileinfo.c \
    tools/hexdump.c \
    tools/strings.c \
    tools/decode.c \
    tools/tools_main.c \
    main.c


OBJ=$(SRC:.c=.o)

all: systemsuite

systemsuite: $(OBJ)
	$(CC) $(OBJ) -o systemsuite $(LDFLAGS)

clean:
	del $(OBJ) systemsuite.exe 2>nul || true
