INCFLAGS = -I..
DEBUG_CCFLAGS = $(INCFLAGS) -c -g -DDEBUG_DATA_DIR="\"../UTData/\"" -D_DEBUG
DEBUG_TARGET_DIR = ../Debug
DEBUG_LIBDIR = -L$(DEBUG_TARGET_DIR)
DEBUG_LIBS = -lbase_g -lbase_l -lm
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_o.a
DEBUG_OBJECTS = Debug/BL_linesearch.o Debug/BL_optimize.o Debug/BL_steepestdescent.o \
	Debug/BL_cg.o
RELEASE_CCFLAGS = $(INCFLAGS) -c -O2 -DNDEBUG
RELEASE_TARGET_DIR = ../Release
RELEASE_LIBDIR = -L$(RELEASE_TARGET_DIR)
RELEASE_LIBS = -lbase_o -lbase_l -lm
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_o.a
RELEASE_OBJECTS = Release/BL_linesearch.o Release/BL_optimize.o Release/BL_steepestdescent.o \
	Release/BL_cg.o

#-----------------------------------------------------------------------------------
# debug
#-----------------------------------------------------------------------------------

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	rm -f $@
	ar -r $@ $^
	ranlib $@

Debug/BL_linesearch.o	:	BL_linesearch.c BL_linesearch.h BL_optimize.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_optimize.o	:	BL_optimize.c BL_optimize.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_steepestdescent.o	:	BL_steepestdescent.c BL_steepestdescent.h BL_optimize.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_cg.o	:	BL_cg.c BL_cg.h BL_optimize.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

clean_debug	:
	rm -f Debug/*.o
	rm -f $(DEBUG_TARGET)

#-----------------------------------------------------------------------------------
# release
#-----------------------------------------------------------------------------------

release	:	$(RELEASE_TARGET)

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	rm -f $@
	ar -r $@ $^
	ranlib $@

Release/BL_linesearch.o	:	BL_linesearch.c BL_linesearch.h BL_optimize.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_optimize.o	:	BL_optimize.c BL_optimize.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_steepestdescent.o	:	BL_steepestdescent.c BL_steepestdescent.h BL_optimize.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_cg.o	:	BL_cg.c BL_cg.h BL_optimize.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

clean_release	:
	rm -f Release/*.o
	rm -f $(RELEASE_TARGET)
