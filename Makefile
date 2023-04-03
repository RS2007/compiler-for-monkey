CC = gcc
C_FLAGS = -Wall
SRC_FOLDER = ./src
TEST_FOLDER = ./tests
BIN_FOLDER = ./bin
SOURCES = $(wildcard $(SRC_FOLDER)/*.c)
HEADERS = $(wildcard $(SRC_FOLDER)/*.h)
TESTS = $(wildcard $(TEST_FOLDER)/*.c)
SOURCES_WITHOUT_MAIN = $(wildcard $(SRC_FOLDER)/[!main]*.c)
BIN = $(wildcard $(BIN_FOLDER)/*)



all: compile
	./bin/mc

compile: $(SOURCES_HEADERS)
	echo $(SOURCES_HEADERS)
	$(CC) -o ./bin/mc $(SOURCES) $(C_FLAGS)

test : $(SOURCES_HEADERS) $(TESTS)
	$(CC) -o ./bin/${name}_test ${TEST_FOLDER}/${name}_test.c ${SOURCES_WITHOUT_MAIN} && ./bin/${name}_test

clean: $(BIN)
	rm -rf $(BIN)

