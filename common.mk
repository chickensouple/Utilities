BUILD_DIR := $(ROOT_DIR)/build
OUTPUT_DIR := $(ROOT_DIR)/bin

LIBRARIES_DIR := $(ROOT_DIR)/libraries
PROGRAMS_DIR := $(ROOT_DIR)/programs
TESTS_DIR := $(ROOT_DIR)/tests

LD_SCRIPT := linker.ld

# compiler
CXX := g++
CC := gcc

# compiler flags
CXX_COMMON_FLAGS:= -O3 -std=c++11 -Wall -Wno-unused-parameter 

# include flags
INCLUDE_COMMON := -I.

OBJECT_PATH := $(subst $(ROOT_DIR), $(ROOT_DIR)/build, $(shell pwd))
OBJECTS := $(addprefix $(OBJECT_PATH)/, $(SOURCES:.cpp=.o))

$(OBJECT_PATH)/%.o: %.cpp
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(dir $@)\1.o|' > $@.d
	@echo "    [$<]"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJECT_PATH)/%.o: %.c
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(dir $@)\1.o|' > $@.d
	@echo "    [$<]"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<



