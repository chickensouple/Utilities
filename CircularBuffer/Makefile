CXX_SOURCES := example.cpp

include ../common.mk

CXXFLAGS := $(CXX_COMMON_FLAGS)
INCLUDES := $(INCLUDE_COMMON)

LDFLAGS := $(LD_COMMON_FLAGS) 

MAIN := $(OUTPUT_DIR)/CircularBufferExample.out

all: $(MAIN) $(OUTPUT_DIR)/BasicTest.out
	@echo "    Built $<"

$(MAIN): $(CXX_OBJECTS)
	@$(CXX) $(CXX_OBJECTS) $(INCLUDES) $(CXXFLAGS) -o $(MAIN)

$(OUTPUT_DIR)/BasicTest.out: $(OBJECT_PATH)/Tests/BasicTest.cpp.o
	@$(CXX) $< $(INCLUDES) $(CXXFLAGS) -o $(OUTPUT_DIR)/BasicTest.out


