LINKER := Assembler

BUILD_DIR := build
SRC_DIRS := core

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS := $(INC_FLAGS) -g -Wall -std=c11

.PHONY: $(LINKER)

$(LINKER): $(BUILD_DIR)/$(LINKER)

$(BUILD_DIR)/%.c.o: %.c
	mkdir $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(LINKER): $(OBJS)
	mkdir $(@D)
	$(CC) $^ -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
