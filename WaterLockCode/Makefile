PRODUCT = waterlock

.phony: all clean test

CC = g++
CFLAGS = -Wall -Werror -Wextra -pedantic -ggdb -O0 -std=c++11 -pthread -Iproduct -Iproduct/interfaces -Iproduct/enums -Iproduct/comLibs -g
#CFLAGS += -Wno-reorder
#CFLAGS += -Wno-unused-parameter

SOURCES = $(wildcard product/*.cpp) \
          $(wildcard product/*/*.c)

HEADERS = $(wildcard product/*.h) \
          $(wildcard product/*/*.h)

TEST = test_$(PRODUCT)
TEST_SOURCES = $(filter-out product/main.cpp, $(SOURCES)) \
			 $(wildcard test/*.cpp)
TEST_HEADERS = $(HEADERS) \
			 $(wildcard test/*.h)
TEST_LIBS = -lgtest -lgmock -lgmock_main -lpthread

all: $(PRODUCT)

$(PRODUCT): $(SOURCES) $(HEADERS) Makefile
	@$(CC) $(CFLAGS) $(SOURCES) -o $@

$(TEST): $(TEST_SOURCES) $(TEST_HEADERS) Makefile
	@$(CC) $(CFLAGS) -Itest $(TEST_SOURCES) -o $@ $(TEST_LIBS)

clean:
	@rm -rf $(PRODUCT) $(TEST)

test: $(TEST)
	@./$(TEST) 2> /dev/null
