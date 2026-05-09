CC := clang
CFLAGS := -I include -Wall -Wextra -g -O0

LDFLAGS := -lSDL3 -lSDL3_image -lm

BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

$(shell mkdir -p $(BUILD_DIR))
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: clean

rpsbr: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf rpsbr