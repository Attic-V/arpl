.PHONY: all clean

TARGET := arpl

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
DEP_DIR := $(BUILD_DIR)/dep
SRC_DIR := src

MODULES := $(patsubst $(SRC_DIR)/%/,%,$(wildcard $(SRC_DIR)/*/))
SRC := $(wildcard $(SRC_DIR)/*/*.c)
OBJ := $(patsubst %,$(OBJ_DIR)/%.o,$(MODULES))
DEP := $(patsubst %,$(DEP_DIR)/%.d,$(MODULES))

CC := gcc

CFLAGS := -std=c23 -Wall -Werror -Wextra -pedantic
CPPFLAGS := -MMD -MP -Iinclude

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ) | $(BUILD_DIR)
	@echo "linking objects"
	@$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%/*.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "compiling '$*'"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(DEP_DIR)/$*.d -c -o $@ $<

$(BUILD_DIR) $(OBJ_DIR) $(DEP_DIR):
	@echo "creating directory '$@'"
	@mkdir -p $@

clean:
	@$(RM) -rv $(BUILD_DIR)

-include $(DEP)

