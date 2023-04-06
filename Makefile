CC = gcc
C_FLAGS = -Wall -g
SRC_FOLDER = ./src
TEST_FOLDER = ./tests
BIN_FOLDER = ./bin
SOURCES = $(wildcard $(SRC_FOLDER)/*.c)
HEADERS = $(wildcard $(SRC_FOLDER)/*.h)
TESTS = $(wildcard $(TEST_FOLDER)/*.c)
SOURCES_WITHOUT_REPL = $(shell find ./src -type f -name '*.c' ! -name 'repl.c')
SOURCES_WITHOUT_MAIN = $(shell find ./src -type f -name '*.c' ! -name 'main.c')
SOURCES_WITHOUT_MAIN_REPL = $(shell find ./src -type f -name '*.c' ! -name 'main.c' ! -name 'repl.c')
BIN = $(wildcard $(BIN_FOLDER)/*)



all: compile
	./bin/mc

compile: $(SOURCES_HEADERS)
	echo $(SOURCES_HEADERS)
	$(CC) -o ./bin/mc $(SOURCES_WITHOUT_REPL) $(C_FLAGS)

repl: $(SOURCES_HEADERS)
	$(CC) -o ./bin/repl $(SOURCES_WITHOUT_MAIN) $(C_FLAGS) && ./bin/repl

test : $(SOURCES_HEADERS) $(TESTS)
	$(CC) -o ./bin/${name}_test ${TEST_FOLDER}/${name}_test.c ${SOURCES_WITHOUT_MAIN_REPL}  ${C_FLAGS} && ./bin/${name}_test

clean: $(BIN)
	rm -rf $(BIN)

