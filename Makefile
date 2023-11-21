# Makefile
# this program relies on the devkitadv (http://devkitadv.sourceforge.net/)
# which must be unpacked, the KITHOME variable must point to its location
KITHOME=/usr/local/devkitadv

# it also relies on my png2gba and raw2gba converters being in the PATH
# https://github.com/IanFinlayson/png2gba/
# https://github.com/IanFinlayson/raw2gba/

# finally it relies on my gba-patcher program to allow the compiled game
# to run on a real GBA -- not needed to run in emulator only
# https://github.com/IanFinlayson/gba-patcher

CC=$(KITHOME)/bin/arm-agb-elf-gcc
CFLAGS=-std=c99 -O3
LFLAGS=-O3 -nostartfiles $(KITHOME)/arm-agb-elf/lib/crt0.o -lm
TARGET=moonpatrol.gba
 
# globs for our types of files
SRCS := $(wildcard source/*.c)
IMGS := $(wildcard images/*.png)
SNDS := $(wildcard audio/*.raw)
HDRS := $(wildcard source/*.h)
MAPS := $(wildcard maps/*.h)

# compute headers and objects we need
GENS := $(patsubst images/%.png,images/%.h,$(IMGS)) $(patsubst audio/%.raw,audio/%.h,$(SNDS))
OBJS := $(patsubst source/%.c,bin/%.o,$(SRCS))

# do everything
all: $(TARGET)
	@echo "All done!"

# link it all together
$(TARGET): $(OBJS) $(HDRS) $(GENS) $(MAPS) Makefile
	@mkdir -p bin
	$(CC) $(LFLAGS) $(OBJS) -o bin/moonpatrol.elf
	$(KITHOME)/bin/arm-agb-elf-objcopy -O binary bin/moonpatrol.elf $(TARGET)
	rm bin/moonpatrol.elf
	gba-patcher $(TARGET)
 
# compile an object based on source and headers
bin/%.o: source/%.c $(HDRS) $(GENS) $(MAPS) Makefile
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

# compile an image into a header
images/%.h: images/%.png
	png2gba -p -t $< && cd ..

# compile an audio file into header data
audio/%.h: audio/%.raw
	raw2gba $< && cd ..
 
# tidy up
clean:
	rm -f $(TARGET) $(OBJS) $(GENS)
	rmdir bin

