CC = gcc
INCFLAGS = -I..
DEBUG_TARGET_DIR = ../Debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_l.a
RELEASE_TARGET_DIR = ../Release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_l.a
DEBUG_OBJCTS = Debug/BL_putils.o Debug/BL_diag.o Debug/BL_futils.o
RELEASE_OBJECTS = Release/BL_putils.o Release/BL_diag.o Release/BL_futils.o

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJCTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar r $@ $^
	ranlib $@

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	ar r $@ $^
	ranlib $@

Debug/BL_putils.o	:	BL_putils.c	BL_putils.h BL_base.h
	mkdir -p Debug
	$(CC) -g -c $(INCFLAGS) -o $@ $<

Debug/BL_diag.o	:	BL_diag.c	BL_diag.h BL_base.h
	mkdir -p Debug
	$(CC) -g -c $(INCFLAGS) -o $@ $<

Debug/BL_futils.o	:	BL_futils.c	BL_futils.h BL_base.h
	mkdir -p Debug
	$(CC) -g -c $(INCFLAGS) -o $@ $<

Release/BL_putils.o	:	BL_putils.c BL_putils.h BL_base.h
	mkdir -p Release
	$(CC) -O2 -c $(INCFLAGS) -o $@ $<

Release/BL_diag.o	:	BL_diag.c BL_diag.h BL_base.h
	mkdir -p Release
	$(CC) -O2 -c $(INCFLAGS) -o $@ $<

Release/BL_futils.o	:	BL_futils.c BL_futils.h BL_base.h
	mkdir -p Release
	$(CC) -O2 -c $(INCFLAGS) -o $@ $<

clean	:
	rm -f Debug/*
	rm -f Release/*
	rm -f $(DEBUG_TARGET)
	rm -f $(RELEASE_TARGET)
