CC = cc
INCFLAGS = -I..
DEBUG_CC_FLAGS = -c -g
DEBUG_TARGET_DIR = ../Debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_cg.a
RELEASE_CC_FLAGS = -c -O2
RELEASE_TARGET_DIR = ../Release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_cg.a
DEBUG_OBJECTS = Debug/BL_oomesh.o Debug/BL_tristrip.o
RELEASE_OBJECTS = Release/BL_oomesh.o Release/BL_tristrip.o

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

clean	:
	rm -f Debug/*
	rm -f Release/*
	rm -f $(DEBUG_TARGET)
	rm -f $(RELEASE_TARGET)