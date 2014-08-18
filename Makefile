CC = g++
CFLAGS = -Wall -g -c
LFLAGS = -g

OBJ_NAMES = piece move board
OBJ_FILES = $(addprefix obj/, $(addsuffix .o, $(OBJ_NAMES)))

#TODO tests, somehow including dependencies

all: test/test_board.cpp $(OBJ_FILES)
	$(CC) $(LFLAGS) -o bin/test_board $^

obj/%.o : src/%.cpp | obj/
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf obj/*
	rm -rf bin/*

