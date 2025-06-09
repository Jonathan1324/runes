UNAME_S := $(shell uname -s)
ARCH ?= x86_64

AS = nasm
ASFLAGS = 
CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

ifeq ($(UNAME_S),Darwin)
	ASFLAGS += -f macho64
	CFLAGS += -arch $(ARCH)
	CXXFLAGS += -arch $(ARCH)
else
	ASFLAGS += -f elf64
	CFLAGS += -m64
	CXXFLAGS += -m64
endif