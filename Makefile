CC:=gcc
CFLAGS:=-Wall
BIN:= main
LOG:= logger.log
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -L. -lconfig

clean:
	$(RM) $(BIN) $(OBJS) $(LOG)