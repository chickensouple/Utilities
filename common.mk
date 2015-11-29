BUILD_DIR := $(ROOT_DIR)/build
OUTPUT_DIR := $(ROOT_DIR)/bin

# compiler
CXX := clang++
CC := gcc

# compiler flags
CXX_COMMON_FLAGS:= -std=c++11 -Wall -Wno-unused-parameter -g 
CC_COMMON_FLAGS := -O3 -Wall -Wno-unused-parameter

# include flags
INCLUDE_COMMON := -I.

# libraries
LIBRARY_BOOST_TEST := -lboost_unit_test_framework

OBJECT_PATH := $(subst $(ROOT_DIR), $(ROOT_DIR)/build, $(shell pwd))
CXX_OBJECTS := $(addprefix $(OBJECT_PATH)/, $(SOURCES:.cpp=.cpp.o))
C_OBJECTS := $(addprefix $(OBJECT_PATH)/, $(SOURCES:.c=.c.o))

CXX_TEST_OBJECTS := $(addprefix $(OBJECT_PATH)/, $(TEST_SOURCES:.cpp=.cpp.o))

$(OBJECT_PATH)/%.cpp.o: %.cpp
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(dir $@)\1.o|' > $@.d
	@echo "    [$<]"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJECT_PATH)/%.c.o: %.c
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(dir $@)\1.o|' > $@.c.d
	@echo "    [$<]"
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<



