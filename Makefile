include build_scripts/config.mk

SRC_DIR = src
BUILD_DIR = build

SRC_DIR := $(shell pwd)/$(SRC_DIR)
BUILD_DIR := $(shell pwd)/$(BUILD_DIR)

.PHONY: all clean src

all: src

src:
	@$(MAKE) -C $(SRC_DIR) 					\
		AS=$(AS) ASFLAGS="$(ASFLAGS)"		\
		CC=$(CC) CFLAGS="$(CFLAGS)" 		\
		CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)"	\
		SRC_DIR=$(SRC_DIR) 					\
		BUILD_DIR=$(BUILD_DIR)

clean:
	@$(MAKE) -C $(SRC_DIR) clean 			\
		SRC_DIR=$(SRC_DIR) 					\
		BUILD_DIR=$(BUILD_DIR)
