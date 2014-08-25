CC = g++
CFLAGS = -Wall -g -c
LFLAGS = -g

#TODO figure out wildcards
OBJ_NAMES = piece move board
OBJ_FILES = $(addprefix obj/, $(addsuffix .o, $(OBJ_NAMES)))

TEST_NAMES = piece move board
TEST_FILES = $(addprefix test/test_, $(TEST_NAMES))
TEST_OBJS = $(addprefix test/obj/test_, $(addsuffix .o, $(TEST_NAMES)))

#TODO somehow include dependencies

all : $(OBJ_FILES)

.PHONY : check
check : bin/unit_tests
	./bin/unit_tests

bin/unit_tests : $(OBJ_FILES) $(TEST_OBJS) test/unit_test.cpp
	$(CC) $(LFLAGS) $^ -o $@

obj/%.o : src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

test/obj/%.o : test/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean :
	rm -rf obj/*
	rm -rf bin/*
	rm -rf test/obj/*

