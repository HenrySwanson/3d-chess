CC = g++
CFLAGS = -Wall -g `wx-config --cxxflags`
LFLAGS = -g `wx-config --libs --gl-libs`

#TODO move unit_test.* to include

CORE_SRCS = $(wildcard src/*.cpp)
CORE_OBJS = $(patsubst %.cpp, %.o, $(CORE_SRCS))

TEST_SRCS = $(wildcard test/*.cpp)
TEST_OBJS = $(patsubst %.cpp, %.o, $(TEST_SRCS))

.PHONY : all check clean

all : bin/3d_chess

check : bin/unit_tests
	./bin/unit_tests

clean :
	rm -rf src/*.o test/*.o
	rm -rf src/*.d test/*.d
	rm -rf bin/*
	rm -rf log/*

bin/3d_chess : $(CORE_OBJS)
	$(CC) $(LFLAGS) $^ -o $@

bin/unit_tests : $(CORE_OBJS) $(TEST_OBJS)
	$(CC) $(LFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -MD -c $< -o $@

ifneq "$(MAKECMDGOALS)" "clean"
  deps  = $(patsubst %.cpp, %.d, $(CORE_SRCS))
  deps += $(patsubst %.cpp, %.d, $(TEST_SRCS))
  -include $(deps)
endif
