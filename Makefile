INCDIR = include
SRCDIR = src
BUILDDIR = build
RANDOM_PORT = $(shell shuf -i 2000-65000 -n 1)
OBJ = $(wildcard $(BUILDDIR)/*.o)
CFLAGS = -g -Wall
INC = -I $(INCDIR)
CC = gcc
PGM = adblock
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))

all: build

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

main: server.o client.o dialog.o utils.o adblock.o
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

build: main

clean:
	@rm -rf $(TARGET)
	@rm -rf $(BUILDDIR)

valgrind: build
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -v $(TARGET)
