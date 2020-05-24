CC = cc
INCFLAGS = -I.. -I/usr/include/opencv4
DEBUG_CC_FLAGS = -c -g
DEBUG_TARGET_DIR = ../Debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_cg.a
RELEASE_CC_FLAGS = -c -O2
RELEASE_TARGET_DIR = ../Release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_cg.a
DEBUG_OBJECTS = Debug/BL_oomesh.o Debug/BL_tristrip.o Debug/BL_glctx.o Debug/BL_glutils.o Debug/BL_die.o \
	Debug/BL_glsurface.o
RELEASE_OBJECTS = Release/BL_oomesh.o Release/BL_tristrip.o Release/BL_glctx.o Release/BL_glutils.o Release/BL_die.o \
	Release/BL_glsurface.o

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar r $@ $(DEBUG_OBJECTS)
	ranlib $@

Debug/BL_oomesh.o	:	BL_oomesh.c BL_oomesh.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_oomesh.c

Debug/BL_tristrip.o	:	BL_tristrip.c BL_tristrip.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_tristrip.c

Debug/BL_glctx.o	:	BL_glctx.c BL_glctx.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_glctx.c

Debug/BL_glutils.o	:	BL_glutils.c BL_glutils.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_glutils.c

Debug/BL_die.o	:	BL_die.cc BL_die.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_die.cc

Debug/BL_glsurface.o	:	BL_glsurface.cc BL_glsurface.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_glsurface.cc

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	ar r $@ $(RELEASE_OBJECTS)
	ranlib $@

Release/BL_oomesh.o	:	BL_oomesh.c BL_oomesh.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_oomesh.c

Release/BL_tristrip.o	:	BL_tristrip.c BL_tristrip.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_tristrip.c

Release/BL_glctx.o	:	BL_glctx.c BL_glctx.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_glctx.c

Release/BL_glutils.o	:	BL_glutils.c BL_glutils.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_glutils.c

Release/BL_die.o	:	BL_die.cc BL_die.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_die.cc

Release/BL_glsurface.o	:	BL_glsurface.cc BL_glsurface.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_glsurface.cc

clean_debug	:
	rm -f Debug/*
	rm -f $(DEBUG_TARGET)

clean_release	:
	rm -f Release/*
	rm -f $(RELEASE_TARGET)