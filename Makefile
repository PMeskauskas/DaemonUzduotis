CC:=gcc
CFLAGS:=-Wall
BIN:= main
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(BIN) $(OBJS)