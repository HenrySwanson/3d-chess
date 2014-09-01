CC = g++
CFLAGS = -Wall -g
LFLAGS = -g

#TODO move unit_test.* to include

CORE_SRCS = $(wildcard src/*.cpp)
CORE_OBJS = $(patsubst %.cpp, %.o, $(CORE_SRCS))

TEST_SRCS = $(wildcard test/*.cpp)
TEST_OBJS = $(patsubst %.cpp, %.o, $(TEST_SRCS))

.PHONY : all check clean

all : $(CORE_OBJS)

check : bin/unit_tests
	./bin/unit_tests

clean :
	rm -rf src/*.o test/*.o
	rm -rf src/*.d test/*.d
	rm  -f bin/unit_tests

bin/unit_tests : $(CORE_OBJS) $(TEST_OBJS)
	$(CC) $(LFLAGS) $^ -o $@

%.o : %.cpp
	$(CC) $(CFLAGS) -MD -c $< -o $@

ifneq "$(MAKECMDGOALS)" "clean"
  deps  = $(patsubst %.cpp, %.d, $(CORE_SRCS))
  deps += $(patsubst %.cpp, %.d, $(TEST_SRCS))
  -include $(deps)
endif
