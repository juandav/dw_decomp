-include local.mk

BUILD_DIR := build
ASM_DIR := asm

PYTHON := python3

.DEFAULT_GOAL := all

include mk/sources.mk
include mk/toolchain/mwcc.mk
include mk/platform/psx.mk
include mk/matching.mk

.EXTRA_PREREQS := $(abspath $(MAKEFILE_LIST))

regenerate: reset
	$(MAKE) generate

clean:
	rm -rf $(BUILD_DIR)

reset: clean
	rm -rf $(ASM_DIR)

-include $(DEP)

.PHONY: all clean
