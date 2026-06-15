# Makefile for NES Emulator on Haiku

# Compiler and flags
CXX = g++
CXXFLAGS = -I./include -I/boot/system/develop/headers/private/interface
LDFLAGS = -lbe -lgame -ltranslation -ltracker -lmedia -llocalestub -lstdc++
OPTFLAG = -O2

# Directories
OBJ_DIR = obj
BIN_DIR = bin

# Source files
MAIN_SRCS = $(wildcard src/*.cpp)
TEST_CPU_SRC = tests/CPU/test_cpu.cpp
TEST_PPU_SRC = tests/PPU/test_load_patterns.cpp
TEST_GUI_SRC = tests/haiku_gui_test.cpp
TEST_SHOW_CHR_SRC = tests/show_chr.cpp

RDEF_FILE = icono.rdef                # cambia al nombre de tu .rdef
RES_OBJ   = $(OBJ_DIR)/icono.rsrc     # archivo compilado de recursos

# Object files
MAIN_OBJS = $(patsubst src/%.cpp,$(OBJ_DIR)/%.o,$(MAIN_SRCS))
TEST_CPU_OBJ = $(OBJ_DIR)/test_cpu.o
TEST_PPU_OBJ = $(OBJ_DIR)/test_load_patterns.o
TEST_GUI_OBJ = $(OBJ_DIR)/haiku_gui_test.o
TEST_SHOW_CHR_OBJ = $(OBJ_DIR)/show_chr.o

ALL_OBJS = $(MAIN_OBJS) $(TEST_CPU_OBJ) $(TEST_PPU_OBJ) $(TEST_GUI_OBJ) $(TEST_SHOW_CHR_OBJ)

# Target executables
TARGET = $(BIN_DIR)/montaNES
TEST_CPU_TARGET = $(BIN_DIR)/test_cpu
TEST_PPU_TARGET = $(BIN_DIR)/test_load_patterns
TEST_GUI_TARGET = $(BIN_DIR)/haiku_gui
TEST_SHOW_CHR_TARGET = $(BIN_DIR)/show_chr

# Default rule
all: $(TARGET) bindcatalogs # $(TEST_CPU_TARGET) $(TEST_PPU_TARGET) $(TEST_GUI_TARGET) $(TEST_SHOW_CHR_TARGET)

# Regla para compilar .rdef a .rsrc ---
$(RES_OBJ): $(RDEF_FILE) | $(OBJ_DIR)
	rc $< -o $@

# Rule to link the main executable
$(TARGET): $(MAIN_OBJS) $(RES_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(MAIN_OBJS) -o $@ $(LDFLAGS) $(OPTFLAG)
	xres -o $@ $(RES_OBJ)

# Rule to link test_cpu
$(TEST_CPU_TARGET): $(TEST_CPU_OBJ) $(OBJ_DIR)/CPU.o $(OBJ_DIR)/opcode.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS) $(OPTFLAG)

# Rule to link test_load_patterns
$(TEST_PPU_TARGET): $(TEST_PPU_OBJ) $(OBJ_DIR)/PPU2.o $(OBJ_DIR)/Cartridge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS) $(OPTFLAG)

# Rule to link haiku_gui
$(TEST_GUI_TARGET): $(TEST_GUI_OBJ) $(MAIN_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS) $(OPTFLAG)

# Rule to link show_chr
$(TEST_SHOW_CHR_TARGET): $(TEST_SHOW_CHR_OBJ) $(OBJ_DIR)/PPU2.o $(OBJ_DIR)/Cartridge.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS) $(OPTFLAG)

# Rule to compile source files into object files in obj/
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPTFLAG)

$(OBJ_DIR)/test_cpu.o: $(TEST_CPU_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPTFLAG)

$(OBJ_DIR)/test_load_patterns.o: $(TEST_PPU_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPTFLAG)

$(OBJ_DIR)/haiku_gui_test.o: $(TEST_GUI_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPTFLAG)

$(OBJ_DIR)/show_chr.o: $(TEST_SHOW_CHR_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(OPTFLAG)

# Clean rule
clean:
	rm -f $(ALL_OBJS) $(RES_OBJ) $(TARGET) $(TEST_CPU_TARGET) $(TEST_PPU_TARGET) $(TEST_GUI_TARGET) $(TEST_SHOW_CHR_TARGET)
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Internationalization (catkeys)
NAME = montaNES
APP_MIME_SIG = x-vnd.Haiku-$(NAME)
CATKEYS_DIR := locales
CATALOGS_DIR := $(OBJ_DIR)/$(APP_MIME_SIG)
LOCALES = en es ja fr de # Add other languages here, e.g., es fr de

# Pseudo-function for converting a list of language codes in LOCALES variable
# to a corresponding list of catkeys files in $(CATALOGS_DIR)/xx.catalog
# The "function" appends the .catalog suffix and prepends the
# $(CATALOGS_DIR)/ path.
define LOCALES_LIST_TO_CATALOGS
        $(addprefix $(CATALOGS_DIR)/, $(addsuffix .catalog, $(foreach lang, $(LOCALES), $(lang))))
endef

CATALOGS = $(LOCALES_LIST_TO_CATALOGS)

# Create the localization sources directory if it doesn't exist.
$(CATKEYS_DIR)::
	@[ -d $(CATKEYS_DIR) ] || mkdir $(CATKEYS_DIR) >/dev/null 2>&1

# Create the localization data directory if it doesn't exist.
$(CATALOGS_DIR):: $(OBJ_DIR)
	@[ -d $(CATALOGS_DIR) ] || mkdir $(CATALOGS_DIR) >/dev/null 2>&1

# Rule to preprocess program sources into file ready for collecting catkeys.
$(OBJ_DIR)/$(NAME).pre : $(MAIN_SRCS) include/KeyConfig.hpp
	@mkdir -p $(OBJ_DIR)
	-cat $(MAIN_SRCS) include/KeyConfig.hpp | $(CXX) -E -x c++ $(CXXFLAGS) -DB_COLLECTING_CATKEYS - | grep -av '^#' > $(OBJ_DIR)/$(NAME).pre

# Rules to collect localization catkeys.
catkeys : $(addprefix $(CATKEYS_DIR)/, $(addsuffix .catkeys, $(LOCALES)))

$(CATKEYS_DIR)/%.catkeys : $(CATKEYS_DIR) $(OBJ_DIR)/$(NAME).pre
	collectcatkeys -s $(APP_MIME_SIG) $(OBJ_DIR)/$(NAME).pre -o $@ -l $(basename $(notdir $@))

# Rule to create localization catalogs.
catalogs : $(CATALOGS_DIR) $(CATALOGS)

# Rule to compile localization data catalogs.
$(CATALOGS_DIR)/%.catalog : $(CATKEYS_DIR)/%.catkeys
	linkcatkeys -o "$ @" -s $(APP_MIME_SIG) -l $(notdir $(basename $ @)) $<

# Alternate way of storing localization catalogs: bind them into the program
# executable's resources.
bindcatalogs :
	$(foreach lc,$(LOCALES),\
		linkcatkeys -o $(TARGET) -s $(APP_MIME_SIG) -tr -l $(lc) $(CATKEYS_DIR)/$(lc).catkeys;\
	)