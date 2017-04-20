INCDIR = include
SRCDIR = src
BUILDDIR = build
RANDOM_PORT = $(shell awk -v min=10000 -v max=65000 'BEGIN{srand(); print int(min+rand()*(max-min+1))}')
OBJ = $(wildcard $(BUILDDIR)/*.o)
ifeq ($(DEBUG),1)
CFLAGS = -g -Wall -DDEBUG=1
else
CFLAGS = -g -Wall -DDEBUG=0
endif
INC = -I $(INCDIR)
CC = gcc
PGM = adblock
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))

all: build

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

main: server.o client.o dialog.o utils.o adblock.o adfilter.o
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

build: main

test: build
	@$(TARGET) --port $(RANDOM_PORT) & sleep 2 ; curl -x 127.0.0.1:$(RANDOM_PORT) http://www.01net.com

clean:
	@rm -rf $(TARGET)
	@rm -rf $(BUILDDIR)

valgrind: build
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -v $(TARGET)
