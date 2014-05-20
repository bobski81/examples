# Simple makefile - experimental
SHELL = /bin/sh

IDIR = src/headers
CC = gcc
CFLAGS = -c -Wall -g -I $(IDIR)
LDFLAGS =

SRCDIR = src
OBJDIR = obj

_OBJECTS = main.o 
OBJECTS = $(patsubst %, $(OBJDIR)/%,$(_OBJECTS))

EXECUTABLE = EX_SERIAL_PORT

# "Make all" command
all: $(EXECUTABLE)

# For project, link all object files together
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

# For each object file, find .c dependencies & compile
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJDIR)/*

# Use this for debug purposes. Call using "make test"
test: 
	@echo $(OBJECTS)
	@echo $(CC) $(CFLAGS) $(OBJECTS)
