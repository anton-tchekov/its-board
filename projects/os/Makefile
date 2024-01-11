PROJECT := sample

CFLAGS += # -DENABLE_RAMDISK

INC += -I src/nano-c/src/ -I src/keyboard/ -I src/ramdisk/ \
	-I src/fatfs/source/

OBJECTS += src/context.o

include ../../lib/lib.mk

src/context.o: src/context.s
	arm-none-eabi-as src/context.s -o src/context.o
