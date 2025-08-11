CC = gcc
CFLAGS = 

LDFLAGS = -lncurses

SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.c)
BUILD_DIR = build
EXE = $(BUILD_DIR)/snake_game

.PHONY: all clean

all: $(EXE)

$(EXE): $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)