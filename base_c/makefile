CC = cc
DEBUG_INCFLAGS = -I..
RELEASE_INCFLAGS = -I..
DEBUG_CCFLAGS = $(DEBUG_INCFLAGS) -c -g -D_DEBUG
RELEASE_CCFLAGS = $(RELEASE_INCFLAGS) -c -O2
DEBUG_TARGET_DIR = ../Debug
RELEASE_TARGET_DIR = ../Release
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_c.a
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_c.a
DEBUG_OBJECTS = Debug/BL_crc.o Debug/BL_packet.o Debug/BL_ringbuffer.o
RELEASE_OBJECTS = Release/BL_crc.o Release/BL_packet.o Release/BL_ringbuffer.o

.PHONY	:	debug

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar -r $@ $(DEBUG_OBJECTS)
	ranlib $@

Debug/BL_crc.o	:	BL_crc.c BL_crc.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ BL_crc.c

Debug/BL_packet.o	:	BL_packet.c BL_packet.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ BL_packet.c

Debug/BL_ringbuffer.o	:	BL_ringbuffer.c BL_ringbuffer.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ BL_ringbuffer.c

.PHONY	:	clean_debug

clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm Debug/BL_crc.o
