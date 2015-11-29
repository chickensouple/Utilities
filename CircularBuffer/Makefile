SOURCES := example.cpp

TEST_SOURCES := Tests/BasicTest.cpp

include ../common.mk

CXXFLAGS := $(CXX_COMMON_FLAGS)
INCLUDES := $(INCLUDE_COMMON)

LDFLAGS := $(LD_COMMON_FLAGS) 

MAIN := $(OUTPUT_DIR)/CircularBufferExample.out

all: $(MAIN) Tests
	@echo "    Built $<"

$(MAIN): $(CXX_OBJECTS)
	@$(CXX) $(CXX_OBJECTS) $(INCLUDES) $(CXXFLAGS) -o $(MAIN)

Tests: $(CXX_TEST_OBJECTS)
	@$(CXX) $(OBJECT_PATH)/Tests/BasicTest.cpp.o $(INCLUDES) $(CXXFLAGS) -o $(OUTPUT_DIR)/BasicTest.out
