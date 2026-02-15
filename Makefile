# Makefile for NES Emulator on Haiku

# Compiler and flags
CXX = g++
CXXFLAGS = -I./include -I/boot/system/develop/headers/private/interface
LDFLAGS = -lbe -lgame -ltranslation

# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Source files
# Source files
MAIN_SRCS = $(wildcard src/*.cpp)
TEST_CPU_SRC = tests/CPU/test_cpu.cpp
TEST_PPU_SRC = tests/PPU/test_load_patterns.cpp
TEST_GUI_SRC = tests/haiku_gui_test.cpp
TEST_SHOW_CHR_SRC = tests/show_chr.cpp

# Object files
MAIN_OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRCS))
TEST_CPU_OBJ = $(OBJ_DIR)/test_cpu.o
TEST_PPU_OBJ = $(OBJ_DIR)/test_load_patterns.o
TEST_GUI_OBJ = $(OBJ_DIR)/haiku_gui_test.o
TEST_SHOW_CHR_OBJ = $(OBJ_DIR)/show_chr.o

ALL_OBJS = $(MAIN_OBJS) $(TEST_CPU_OBJ) $(TEST_PPU_OBJ) $(TEST_GUI_OBJ) $(TEST_SHOW_CHR_OBJ)

# Target executables
TARGET = $(BIN_DIR)/nes_emulator
TEST_CPU_TARGET = $(BIN_DIR)/test_cpu
TEST_PPU_TARGET = $(BIN_DIR)/test_load_patterns
TEST_GUI_TARGET = $(BIN_DIR)/haiku_gui
TEST_SHOW_CHR_TARGET = $(BIN_DIR)/show_chr

# Default rule
all: $(TARGET) # $(TEST_CPU_TARGET) $(TEST_PPU_TARGET) $(TEST_GUI_TARGET) $(TEST_SHOW_CHR_TARGET)

# Rule to link the main executable
$(TARGET): $(MAIN_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to link test_cpu
$(TEST_CPU_TARGET): $(TEST_CPU_OBJ) $(OBJ_DIR)/CPU.o $(OBJ_DIR)/opcode.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to link test_load_patterns
$(TEST_PPU_TARGET): $(TEST_PPU_OBJ) $(OBJ_DIR)/PPU2.o $(OBJ_DIR)/Cartridge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to link haiku_gui
$(TEST_GUI_TARGET): $(TEST_GUI_OBJ) $(MAIN_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to link show_chr
$(TEST_SHOW_CHR_TARGET): $(TEST_SHOW_CHR_OBJ) $(OBJ_DIR)/PPU2.o $(OBJ_DIR)/Cartridge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Rule to compile source files into object files in obj/
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_cpu.o: $(TEST_CPU_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/test_load_patterns.o: $(TEST_PPU_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/haiku_gui_test.o: $(TEST_GUI_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/show_chr.o: $(TEST_SHOW_CHR_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(ALL_OBJS) $(TARGET) $(TEST_CPU_TARGET) $(TEST_PPU_TARGET) $(TEST_GUI_TARGET) $(TEST_SHOW_CHR_TARGET)
	rm -rf $(OBJ_DIR) $(BIN_DIR)
