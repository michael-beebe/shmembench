

CC = oshcc
CFLAGS = -std=gnu11 -Wall -I./src/include -O2
LDFLAGS =

SRC_DIR = ./src
RS_SRC_DIR = ./rs
INCLUDE_DIR = $(SRC_DIR)/include
BENCHMARKS_DIR = $(SRC_DIR)/benchmarks

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

OBJS = $(SRCS:.c=.o)

TARGET = shmembench

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(RS_SRC_DIR)/target

rs-bin:
	cd $(RS_SRC_DIR) && cargo build --release

.PHONY: all clean
