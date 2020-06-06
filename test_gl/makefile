INCFLAGS = -I.. -I/usr/include/opencv4
DEBUG_TARGET_DIR = ../Debug
RELEASE_TARGET_DIR = ../Release
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/test_gl.exe
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/test_gl.exe
DEBUG_OBJECTS = Debug/BL_glctx.o Debug/BL_glxctx.o Debug/BL_glxcommon.o Debug/main.o
RELEASE_OBJECTS = Release/BL_glctx.o Release/BL_glxctx.o Release/BL_glxcommon.o Release/main.o
OCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lstdc++ -lpthread -lm
DEBUG_LIBS = -lbase_l -lGL -lxcb-glx
DEBUG_LIB_DIR = -L$(DEBUG_TARGET_DIR)
RELEASE_LIB_DIR = -L$(RELEASE_TARGET_DIR)
DEBUG_CCFLAGS = -c -g $(INCFLAGS)

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	$(CC) -o $@ $^ $(DEBUG_LIB_DIR) $(DEBUG_LIBS)

Debug/BL_glctx.o	:	BL_glctx.c BL_glctx.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_glxctx.o	:	BL_glxctx.c BL_glxctx.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_glxcommon.o	:	BL_glxcommon.c BL_glxcommon.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/main.o	:	main.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

clean_debug	:
	rm -f $(DEBUG_TARGET)
	rm -f Debug/*.o

