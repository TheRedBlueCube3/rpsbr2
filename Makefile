CC := clang
CFLAGS := -I include -Wall -Wextra
CFLAGS_DBG := $(CFLAGS) -g -O0
CFLAGS_REL := $(CFLAGS) -O2

LDFLAGS := -lSDL3 -lSDL3_image -lSDL3_ttf -lm
LDFLAGS_REL := $(LDFLAGS) -s

BUILD_DIR := build
SRC_DIR := src
INCLUDE_DIR := include

APPLICATION := rpsbr
APPLICATION_DBG := rpsbr-debug

$(shell mkdir -p $(BUILD_DIR))
SRCS := $(wildcard $(SRC_DIR)/*.c)
DBG_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/dbg_%.o,$(SRCS))
REL_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/rel_%.o,$(SRCS))

.PHONY: all debug release clean

all: release

debug: $(APPLICATION_DBG)
release: $(APPLICATION)
both: debug release

$(APPLICATION): $(REL_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS_REL)

$(APPLICATION_DBG): $(DBG_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/dbg_%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS_DBG)

$(BUILD_DIR)/rel_%.o: $(SRC_DIR)/%.c
	$(CC) -c -o $@ $^ $(CFLAGS_REL)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(APPLICATION)
	rm -rf $(APPLICATION_DBG)