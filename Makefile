INCDIR = include
SRCDIR = src
BUILDDIR = build
OBJ = $(wildcard $(BUILDDIR)/*.o)

INC = -I $(INCDIR)
CC = gcc
PGM = adblock
TARGET = ./$(PGM)

$(shell mkdir -p $(BUILDDIR))

all: build

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $(BUILDDIR)/$@

main: server.o client.o dialog.o adblock.o
	$(CC) $(CFLAGS) $(OBJ) $(LIBFLAGS) -o $(TARGET)

build: main

clean: deleteFiles
	@rm -rf $(TARGET)
	@rm -rf $(BUILDDIR)


#valgrind: build
#	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -v $(TARGET)
