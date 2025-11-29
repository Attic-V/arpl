.PHONY: all clean

TARGET := arpl

BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
DEP_DIR := $(BUILD_DIR)/dep
SRC_DIR := src
MOD_DIR := $(SRC_DIR)/modules

MAIN := main

MODULES := $(patsubst $(MOD_DIR)/%/,%,$(wildcard $(MOD_DIR)/*/))
MOD_SRC := $(wildcard $(MOD_DIR)/*/*.c)
OBJ := $(patsubst %,$(OBJ_DIR)/%.o,$(MODULES))
DEP := $(patsubst %,$(DEP_DIR)/%.d,$(MODULES))

CC := gcc

CFLAGS := -std=c23 -Wall -Werror -Wextra -pedantic
CPPFLAGS := -MMD -MP -Iinclude
LDFLAGS := -lm

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ) $(OBJ_DIR)/$(MAIN).o | $(BUILD_DIR)
	@echo "linking objects"
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(MOD_DIR)/%/*.c | $(OBJ_DIR) $(DEP_DIR)
	@echo "compiling '$*'"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(DEP_DIR)/$*.d -c -o $@ $<

$(OBJ_DIR)/$(MAIN).o: $(SRC_DIR)/$(MAIN).c | $(OBJ_DIR) $(DEP_DIR)
	@echo "compiling '$(MAIN).c'"
	@$(CC) $(CPPFLAGS) $(CFLAGS) -MF $(DEP_DIR)/$(MAIN).d -c -o $@ $<

$(BUILD_DIR) $(OBJ_DIR) $(DEP_DIR):
	@echo "creating directory '$@'"
	@mkdir -p $@

clean:
	@$(RM) -rv $(BUILD_DIR)

-include $(DEP)

