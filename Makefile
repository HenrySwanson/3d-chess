CC = g++
CFLAGS = -Wall -g -c
LFLAGS = -g

OBJ_NAMES = piece move board
OBJ_FILES = $(addprefix obj/, $(addsuffix .o, $(OBJ_NAMES)))

TEST_NAMES = piece move board
TEST_FILES = $(addprefix test/test_, $(TEST_NAMES))
TEST_BINS = $(addprefix bin/test_, $(TEST_NAMES))

#TODO tests, somehow including dependencies

all : $(TEST_BINS)

ut : $(OBJ_FILES)
	$(CC) $(LFLAGS) test/unit_test.cpp test/test_piece.cpp $^ -o bin/ut

bin/test_% : test/test_%.cpp $(OBJ_FILES)
	$(CC) $(LFLAGS) -o $@ $^


obj/%.o : src/%.cpp | obj/
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm -rf obj/*
	rm -rf bin/*

