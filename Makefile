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

# Data type configuration
DATATYPE ?= long
ifeq ($(DATATYPE), int)
    CFLAGS += -DBENCHMARK_DATATYPE=int -DBENCHMARK_DATATYPE_NAME=\"int\"
else ifeq ($(DATATYPE), long)
    CFLAGS += -DBENCHMARK_DATATYPE=long -DBENCHMARK_DATATYPE_NAME=\"long\"
else ifeq ($(DATATYPE), double)
    CFLAGS += -DBENCHMARK_DATATYPE=double -DBENCHMARK_DATATYPE_NAME=\"double\"
else ifeq ($(DATATYPE), float)
    CFLAGS += -DBENCHMARK_DATATYPE=float -DBENCHMARK_DATATYPE_NAME=\"float\"
else ifeq ($(DATATYPE), char)
    CFLAGS += -DBENCHMARK_DATATYPE=char -DBENCHMARK_DATATYPE_NAME=\"char\"
else ifeq ($(DATATYPE), short)
    CFLAGS += -DBENCHMARK_DATATYPE=short -DBENCHMARK_DATATYPE_NAME=\"short\"
else
    $(error Unsupported DATATYPE: $(DATATYPE). Supported types: int, long, double, float, char, short)
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

# Show help information
help:
	@echo "Available targets:"
	@echo "  all        - Build the benchmark (default)"
	@echo "  clean      - Remove build artifacts"
	@echo "  rs-bin     - Build Rust binaries"
	@echo "  debug      - Print build variables"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Available variables:"
	@echo "  DATATYPE   - Data type for benchmarks (default: long)"
	@echo "               Supported: int, long, double, float, char, short"
	@echo "  USE_SHMEM_VERSION - OpenSHMEM version (default: 15)"
	@echo "                      Supported: 14, 15"
	@echo ""
	@echo "Examples:"
	@echo "  make DATATYPE=int              # Build with int data type"
	@echo "  make DATATYPE=double           # Build with double data type"
	@echo "  make USE_SHMEM_VERSION=14      # Build for OpenSHMEM 1.4"
	@echo "  make DATATYPE=float USE_SHMEM_VERSION=14  # Combine options"

.PHONY: all clean rs-bin debug help
