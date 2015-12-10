CXX_SOURCES := example.cpp

include ../common.mk

CXXFLAGS := $(CXX_COMMON_FLAGS)
INCLUDES := $(INCLUDE_COMMON)

LDFLAGS := $(LD_COMMON_FLAGS) 

MAIN := $(OUTPUT_DIR)/CircularBufferExample.out

all: $(MAIN)
	@echo "    Built $<"

$(MAIN): $(CXX_OBJECTS)
	@$(CXX) $(CXX_OBJECTS) $(INCLUDES) $(CXXFLAGS) -o $(MAIN)


