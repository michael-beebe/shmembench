CC = oshcc
CFLAGS = -std=gnu11 -Wall -I./src/include -O2
LDFLAGS =

SRC_DIR = ./src
RS_SRC_DIR = ./rs
INCLUDE_DIR = $(SRC_DIR)/include
BENCHMARKS_DIR = $(SRC_DIR)/benchmarks
BUILD_DIR = ./build
BIN_DIR = $(BUILD_DIR)/bin

USE_SHMEM_VERSION ?= 15
ifeq ($(USE_SHMEM_VERSION), 14)
    CFLAGS += -DUSE_14
else ifeq ($(USE_SHMEM_VERSION), 15)
    CFLAGS += -DUSE_15
endif

SRCS = $(wildcard $(SRC_DIR)/*.c) \
       $(wildcard $(BENCHMARKS_DIR)/atomics/*.c) \
       $(wildcard $(BENCHMARKS_DIR)/collectives/*.c) \
       $(wildcard $(BENCHMARKS_DIR)/rma/*.c)

# Create build object paths
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/shmembench

.PHONY: all clean rs-bin

all: $(TARGET)

# Rule for compiling any source file
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(RS_SRC_DIR)/target

rs-bin:
	cd $(RS_SRC_DIR) && cargo build --release

# Print variables for debugging
debug:
	@echo "SRCS = $(SRCS)"
	@echo "OBJS = $(OBJS)"
	@echo "TARGET = $(TARGET)"

.PHONY: all clean rs-bin debug
