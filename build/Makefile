CC = gcc

SRC_DIR = ../src
BIN_DIR = ../bin

SOURCES = $(SRC_DIR)/*.c
HEADERS = $(SRC_DIR)/*.h

TARGET = $(BIN_DIR)/shell

all: clean build run

build:
	mkdir -p $(BIN_DIR) && \
	$(CC) $(SOURCES) -g -I$(HEADERS) -o $(TARGET)

clean:
	rm -rf $(BIN_DIR)

run:
	$(TARGET)