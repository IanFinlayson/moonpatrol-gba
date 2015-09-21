
# this program relies on the devkitadv (ttp://devkitadv.sourceforge.net/)
# which must be unpacked, the KITHOME variable must point to its location
KITHOME="/home/finlayson/gba/devkitadv/"

CC=$(KITHOME)/bin/arm-agb-elf-gcc
CFLAGS=-O3
LFLAGS=-O3 -nostartfiles $(KITHOME)/arm-agb-elf/lib/crt0.o -lm
TARGET=moonbase.gba
 
# globs
SRCS := $(wildcard source/*.c)
HDRS := $(wildcard source/*.h)
OBJS := $(patsubst source/%.c,bin/%.o,$(SRCS))

# do everything
all: tags $(TARGET)
	@echo "All done!"

# link it all together
$(TARGET): $(OBJS) $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(LFLAGS) $(OBJS) -o bin/moonbase.elf
	$(KITHOME)/bin/arm-agb-elf-objcopy -O binary bin/moonbase.elf $(TARGET)
 
# compile an object based on source and headers
bin/%.o: source/%.c $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@
 
# tidy up
clean:
	rm -f $(TARGET) $(OBJS) bin/moonbase.elf tags

# create ctags index
tags:
	ctags $(SRCS) $(HDRS)

