CC        := arm-none-eabi-gcc
CFLAGS    := -Wall -Wextra -ffreestanding -nostdlib -nostartfiles \
	-nodefaultlibs -O2 -std=c99 -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 \
	-D STM32F429xx

BINDIR    := bin
SRCDIR    := src
INCDIR    := src
OBJDIR    := obj
TARGET    := $(BINDIR)/$(PROJECT)

LIBDIR    := ../../lib
LIBSRCDIR := $(LIBDIR)/src
LIBINCDIR := $(LIBDIR)/src
LIBOBJDIR := $(LIBDIR)/obj

INC       := -I $(LIBINCDIR) -I $(INCDIR) \
             -I $(LIBINCDIR)/stm32f4xx_hal/inc/ \

all:     $(TARGET).hex

clean:
	rm $(LIBOBJDIR)/* $(OBJDIR)/* $(TARGET) -rf

flash: upload

upload: $(TARGET).hex
	st-flash --format ihex write $(TARGET).hex

dump: disasm

disasm:
	arm-none-eabi-objdump -d $(TARGET).elf

SOURCES := $(shell find $(SRCDIR) -type f -name *.c*)
HEDEARS := $(shell find $(INCDIR) -type f -name *.h*)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(addsuffix .o,$(basename $(SOURCES))))
DEPS    := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(addsuffix .d,$(basename $(SOURCES))))

$(OBJDIR)/%.o: $(SRCDIR)/%.c*
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INC) -M $< -MT $@ > $(@:.o=.td)
	@cp $(@:.o=.td) $(@:.o=.d);
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d);
	@rm -f $(@:.o=.td)

LIBSOURCES := $(shell find $(LIBSRCDIR) -type f | grep [.]c)
LIBHEDEARS := $(shell find $(LIBINCDIR) -type f | grep [.]h)
LIBOBJECTS := $(patsubst $(LIBSRCDIR)/%,$(LIBOBJDIR)/%,$(addsuffix .o,$(basename $(LIBSOURCES))))
LIBDEPS    := $(patsubst $(LIBSRCDIR)/%,$(LIBOBJDIR)/%,$(addsuffix .d,$(basename $(LIBSOURCES))))

$(LIBOBJDIR)/%.o: $(LIBSRCDIR)/%.c*
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INC) -M $< -MT $@ > $(@:.o=.td)
	@cp $(@:.o=.td) $(@:.o=.d);
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.td) >> $(@:.o=.d);
	@rm -f $(@:.o=.td)

$(TARGET).hex: $(TARGET).elf
	arm-none-eabi-objcopy -S -O ihex $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(OBJECTS) $(LIBOBJECTS) $(LIBOBJDIR)/startup.o
	@mkdir -p bin/
	@$(CC) -nostartfiles -nostdlib -T $(LIBDIR)/linker.ld \
		$(LIBOBJDIR)/startup.o $(OBJECTS) $(LIBOBJECTS) \
		-o $(TARGET).elf \

$(LIBOBJDIR)/startup.o: $(LIBSRCDIR)/startup.s
	arm-none-eabi-as $(LIBSRCDIR)/startup.s -o $(LIBOBJDIR)/startup.o

-include $(DEPS)

.PHONY: clean all upload debug release

