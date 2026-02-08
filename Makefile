# Makefile for NES Emulator on Haiku

# Compiler and flags
CXX = g++
CXXFLAGS = -I./include -I/boot/system/develop/headers/private/interface
LDFLAGS = -lbe -lgame -ltranslation

# Source files
SRCS = src/NES.cpp src/CPU.cpp src/Cartridge.cpp src/PPU2.cpp src/opcode.cpp tests/haiku_gui_test.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target executable
TARGET = nes_emulator

# Default rule
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
