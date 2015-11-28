SOURCES := main.cpp

include ../common.mk

CXXFLAGS := $(CXX_COMMON_FLAGS)
INCLUDES := $(INCLUDE_COMMON)

LDFLAGS := $(LD_COMMON_FLAGS) 

MAIN := $(OUTPUT_DIR)/main.out

all: $(MAIN)
	@echo "    Built $<"

$(MAIN): $(OBJECTS)
	@$(CXX) $(OBJECTS) $(INCLUDES) $(CXXFLAGS) -o $(MAIN)


