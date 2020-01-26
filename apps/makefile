WORK_DIR = ..
DEBUG_CCFLAGS = -c -g -DAPPDATA="\"$(WORK_DIR)/APPDATA/\""
RELEASE_CCFLAGS = -c -O2 -DAPPDATA="\"$(WORK_DIR)/APPDATA/\""
DEBUG_TARGET = $(WORK_DIR)/Debug/app0.exe
RELEASE_TARGET = $(WORK_DIR)/Release/app0.exe
DEBUG_LIBDIR = -L$(DEBUG_TARGET)
DEBUG_LIBFLAGS = $(DEBUG_LIBDIR) -lbase_c -lbase_c -lpthread -lm
RELEASE_LIBDIR = -L$(RELEASE_TARGET)
RELEASE_LIBFLAGS = $(RELEASE_LIBDIR) -lbase_c -lbase_c -lpthread -lm
INCFLAGS = -I$(WORK_DIR)

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

DEBUG_OBJECTS = app0.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIBFLAGS)

Debug/app0.o	:	app0.c
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) $(INCFLAGS) -o $@ $?

clean_debug	:
	rm -f Debug/*.o
	rm -f $(DEBUG_TARGET)

clean_release	:
	rm -f Release/*.o
	rm -f (RELEASE_TARGET)
