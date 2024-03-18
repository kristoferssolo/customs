CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O3 -DNDEBUG

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
TEST_DIR = tests

SRC_FILES := $(wildcard $(SRC_DIR)/*.cc)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRC_FILES))

EXECUTABLE = $(BIN_DIR)/main

.PHONY: build clean run test

build: $(EXECUTABLE)
	@echo "Build complete."

$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


run: $(EXECUTABLE)
	./$(EXECUTABLE)

test:
	@echo "Running tests..."
	@cd $(TEST_DIR) && $(MAKE) test

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned up."
