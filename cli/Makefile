REPO_ROOT := $(CURDIR)/..
BIN_PATH := $(HOME)/.local/bin

SRC := $(wildcard src/*.c)
TARGET := workaholic
FLAGS := -Wall -Wextra -O2 -Wpedantic -std=c17


path: build
	mkdir -p $(BIN_PATH)
	mv $(TARGET) $(BIN_PATH)/$(TARGET)

build:
	gcc $(FLAGS) $(SRC) -o $(TARGET)

path-clean:
	rm -f $(BIN_PATH)/$(TARGET)


clean:
	rm -f ${TARGET}
