# Makefile
# this program relies on the devkitadv (http://devkitadv.sourceforge.net/)
# which must be unpacked, the KITHOME variable must point to its location
# it also relies on my png2gba image converter being in the PATH
# (https://github.com/IanFinlayson/moonpatrol-gba)
KITHOME=/home/finlayson/source/devkitadv

CC=$(KITHOME)/bin/arm-agb-elf-gcc
CFLAGS=-std=c99 -O3
LFLAGS=-O3 -nostartfiles $(KITHOME)/arm-agb-elf/lib/crt0.o -lm
TARGET=moonpatrol.gba
 
# globs
SRCS := $(wildcard source/*.c)
HDRS := $(wildcard source/*.h) images/background.h images/objects.h
OBJS := $(patsubst source/%.c,bin/%.o,$(SRCS))

# do everything
all: tags $(TARGET)
	@echo "All done!"

# link it all together
$(TARGET): $(OBJS) $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(LFLAGS) $(OBJS) -o bin/moonpatrol.elf
	$(KITHOME)/bin/arm-agb-elf-objcopy -O binary bin/moonpatrol.elf $(TARGET)
 
# compile an object based on source and headers
bin/%.o: source/%.c $(HDRS) Makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# compile the images into header data
images/background.h: images/background.png
	cd images && png2gba -p -t background.png && cd ..

images/objects.h: images/objects.png
	cd images && png2gba -p -t objects.png && cd ..
 
# tidy up
clean:
	rm -f $(TARGET) $(OBJS) bin/moonpatrol.elf tags

# create ctags index
tags: $(SRCS) $(HDRS)
	ctags $(SRCS) $(HDRS)

