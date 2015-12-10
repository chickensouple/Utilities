BUILD_DIR := $(ROOT_DIR)/build
OUTPUT_DIR := $(ROOT_DIR)/bin
CURR_DIR := $(shell pwd)
PERL_CURR_DIR := $(subst /,\/,$(CURR_DIR))
PERL_BUILD_DIR := $(subst /,\/,$(BUILD_DIR))

# dependency files
DEP_FILES := $(shell find $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST)))) -name '*.d')
-include $(DEP_FILES)

# compiler
CXX := clang++
CC := gcc

# compiler flags
CXX_COMMON_FLAGS:= -O3 -std=c++11 -Wall -Wno-unused-parameter
CC_COMMON_FLAGS := -O3 -Wall -Wno-unused-parameter

# include flags
INCLUDE_COMMON := -I.

# libraries
LIBRARY_BOOST_TEST := -lboost_unit_test_framework

OBJECT_PATH := $(subst $(ROOT_DIR), $(BUILD_DIR), $(shell pwd))
CXX_OBJECTS := $(addprefix $(OBJECT_PATH)/, $(CXX_SOURCES:.cpp=.cpp.o))
C_OBJECTS := $(addprefix $(OBJECT_PATH)/, $(C_SOURCES:.c=.c.o))

$(OBJECT_PATH)/%.cpp.o: %.cpp
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.((?!o)[a-zA-Z]*)/$(PERL_CURR_DIR)\/$$1.$$2/g' | \
		perl -pe 's/([a-zA-Z0-9_\/-]*)\.o/$(subst /,\/,$(dir $@))$$1.cpp.o/g' > $@.d
	@echo "    [$@]"	
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

$(OBJECT_PATH)/%.c.o: %.c
# mkdir will recreate the file structure of the original
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CXXFLAGS) $(INCLUDES) $< | sed 's|\([a-zA-Z0-9_-]*\)\.o|$(dir $@)\1.o|' > $@.c.d
	@echo "    [$@]"
	@$(CC) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<



