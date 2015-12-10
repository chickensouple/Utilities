export MAKEFLAGS=--no-print-directory

ROOT_DIR := $(shell pwd)
export ROOT_DIR

PROJECTS := CircularBuffer Matrix

all: directories
	@for dir in $(PROJECTS); do \
	echo $$(basename $$dir) ; $(MAKE) $(SILENT) -C $$dir all || exit 2; done

include common.mk

clean:
	rm -f -r $(BUILD_DIR)
	rm -f -r $(OUTPUT_DIR)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OUTPUT_DIR)

.PHONY: all clean directories
