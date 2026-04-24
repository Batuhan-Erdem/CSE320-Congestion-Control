CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/main.c src/node.c src/routing.c src/congestion.c
OUT = node

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)