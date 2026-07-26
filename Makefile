SRC_DIR   := src
OBJ_DIR   := obj
TEST_DIR  := test
UNITY_DIR := $(TEST_DIR)/unity
GCOVR         := gcovr
COVERAGE_DIR  := build/coverage
GCOVR_FLAGS   := --root . \
                 --filter '$(SRC_DIR)/' \
                 --exclude '$(UNITY_DIR)/' \
                 --exclude '$(TEST_DIR)/'



EXE       := main
TEST_EXE  := run_tests

SRC       := $(wildcard $(SRC_DIR)/*.c)
OBJ       := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_SRC  := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ  := $(TEST_SRC:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
UNITY_OBJ := $(OBJ_DIR)/unity.o

# Exclude your app's main file so it doesn't conflict with Unity's main()
APP_OBJ_NO_MAIN := $(filter-out $(OBJ_DIR)/main.o, $(OBJ))

# Added Unity include path so compilation doesn't fail on #include "unity.h"
CPPFLAGS  := -Iinclude -I$(UNITY_DIR) -MMD -MP
CFLAGS    := -Wall -g --coverage

# NEW: Linker flags to resolve the LLVM profile runtime symbols
LDFLAGS   := --coverage
TEST_LDFLAGS := --coverage

.PHONY: all test clean coverage coverage-clean

all: $(EXE)

# Added execution step so running 'make test' actually executes the tests
test: $(TEST_EXE)
	./$(TEST_EXE)

# UPDATED: Added $(TEST_LDFLAGS) to the link command
$(TEST_EXE): $(UNITY_OBJ) $(APP_OBJ_NO_MAIN) $(TEST_OBJ)
	$(CC) $(TEST_LDFLAGS) $^ -o $@

$(UNITY_OBJ): $(UNITY_DIR)/unity.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Notice the pattern match change here from % to test_% to prevent rule collision
$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# UPDATED: Added $(LDFLAGS) to the link command
$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

coverage: test
	@mkdir -p $(COVERAGE_DIR)
	@echo "=== Coverage Summary ==="
	$(GCOVR) $(GCOVR_FLAGS)
	@echo "=== Generating HTML Report ==="
	$(GCOVR) $(GCOVR_FLAGS) --html-details -o $(COVERAGE_DIR)/index.html
	@echo "Report ready at: $(COVERAGE_DIR)/index.html"

coverage-clean:
	@$(RM) -rf $(COVERAGE_DIR)
	@find $(OBJ_DIR) -name "*.gcda" -delete
	@echo "Cleaned coverage raw artifacts and reports."
	
clean:
	@$(RM) -rv $(EXE) $(TEST_EXE) $(OBJ_DIR) $(COVERAGE_DIR)

-include $(OBJ:.o=.d) $(TEST_OBJ:.o=.d)