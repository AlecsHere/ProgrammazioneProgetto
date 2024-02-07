CC = g++
FLAGS = -g -Wall -lncurses -I$(IDIR)

IDIR = includes
SRCDIR = src
OBJDIR = obj

SOURCES = $(SRCDIR)/*
OBJS = $(addprefix $(OBJDIR), $(SOURCES:cpp=o))

.PHONY = all run clean

all: main

main: $(SOURCES)
	$(CC) $(SOURCES) $(FLAGS) -o main

run:
	./main

clean:
	rm -f main $(OBJDIR)/*