CROSS_COMPILE ?= arm-linux-gnuenbihf-
TARGET ?= bsp

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
OBJCOPY := $(CORSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump

INCDIRS := bsp/inc
SRCDIRS := bsp/src

INCLUDE := $(patsubst %, -I%, $(INCDIRS))


INCLUDE : bsp;

SOURCE = $(wildcard *.c)
