CC = clang-13
DEBUGGER = gdb
C_FLAGS = -Wall -g -ggdb3 -pedantic
SRC_FOLDER = ./src
TEST_FOLDER = ./tests
BIN_FOLDER = ./bin
TARGET = -x86-64-amd64-linux
SOURCES = $(wildcard $(SRC_FOLDER)/*.c)
HEADERS = $(wildcard $(SRC_FOLDER)/*.h)
TESTS = $(wildcard $(TEST_FOLDER)/*.c)
SOURCES_WITHOUT_REPL_EVALUATOR_OBJECT = $(shell find ./src -type f -name '*.c' ! -name 'repl.c' ! -name 'evaluator.c' ! -name 'object.c')
SOURCES_WITHOUT_MAIN = $(shell find ./src -type f -name '*.c' ! -name 'main.c' ! -name 'parser.c')
SOURCES_FOR_REPL = $(shell find ./src -type f -name '*.c' ! -name 'main.c')
SOURCES_WITHOUT_MAIN_REPL = $(shell find ./src -type f -name '*.c' ! -name 'main.c' ! -name 'repl.c')
BIN = $(wildcard $(BIN_FOLDER)/*)



all: compile
	./bin/mc

compile: $(SOURCES_HEADERS)
	echo $(SOURCES_HEADERS)
	$(CC) -o ./bin/mc $(SOURCES_WITHOUT_REPL) $(C_FLAGS) -target $(TARGET)

repl: $(SOURCES_HEADERS)
	$(CC) -o ./bin/repl $(SOURCES_FOR_REPL) $(C_FLAGS) -target $(TARGET) && ./bin/repl

test : $(SOURCES_HEADERS) $(TESTS)
	$(CC) -o ./bin/${name}_test ${TEST_FOLDER}/${name}_test.c ${SOURCES_WITHOUT_MAIN_REPL}  ${C_FLAGS} -target $(TARGET) && ./bin/${name}_test

pretty: $(SOURCES) $(HEADERS)
	./format.sh

test-debug: $(SOURCES_HEADERS) $(TESTS)
	$(CC) -o ./bin/${name}_test ${TEST_FOLDER}/${name}_test.c ${SOURCES_WITHOUT_MAIN_REPL}  ${C_FLAGS} -target $(TARGET) && gdb ./bin/${name}_test

clean: $(BIN)
	rm -rf $(BIN)


temp: $(SOURCES_HEADERS) $(TESTS)
	$(CC) -o ./bin/parser_test ${TEST_FOLDER}/parser_test.c ${SOURCES_WITHOUT_MAIN_REPL} $(C_FLAGS) && ./bin/parser_test
