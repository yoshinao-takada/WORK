INCFLAGS = -I.. -I/usr/include/opencv4
DEBUG_TARGET_DIR = ../../Debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/UTbase_g.exe
DEBUG_DEPENDS = $(DEBUG_TARGET_DIR)/libbase_l.a $(DEBUG_TARGET_DIR)/libbase_g.a
OCV_LIBS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
GTEST_LIBS = -lgtest -lgtest_main
DEBUG_CCFLAGS = $(INCFLAGS) -c -g
DEBUG_LIBFLAGS = -L$(DEBUG_TARGET_DIR) -lbase_g -lbase_l $(GTEST_LIBS) $(OCV_LIBS) -lstdc++ -lm
RELEASE_TARGET_DIR = ../../Release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/UTbase_g.exe
RELEASE_DEPENDS = $(RELEASE_TARGET_DIR)/libbase_l.a $(RELEASE_TARGET_DIR)/libbase_g.a
RELEASE_CCFLAGS = $(INCFLAGS) -c -O2
RELEASE_LIBFLAGS = -L$(RELEASE_TARGET_DIR) -lbase_g -lbase_l $(GTEST_LIBS) $(OCV_LIBS) -lstdc++ -lm

DEBUG_OBJECTS = Debug/mat.o Debug/interp.o Debug/discriminate.o Debug/invremap.o
RELEASE_OBJECTS = Release/mat.o Release/interp.o Release/discriminate.o Release/invremap.o

all	:
	$(MAKE) clean
	$(MAKE) debug
	$(MAKE) release

#----------------------------------------------
# debug rules
#----------------------------------------------
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIBFLAGS)

Debug/mat.o	:	mat.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o$@ $<

Debug/interp.o	:	interp.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o$@ $<

Debug/discriminate.o	:	discriminate.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o$@ $<

Debug/invremap.o	:	invremap.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o$@ $<

clean_debug	:
	rm -rf Debug/*.o
	rm -rf $(DEBUG_TARGET)

#----------------------------------------------
# release rules
#----------------------------------------------
release	:	$(RELEASE_TARGET)

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $(RELEASE_OBJECTS) $(RELEASE_LIBFLAGS)

Release/mat.o	:	mat.cc
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o$@ $<

Release/interp.o	:	interp.cc
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o$@ $<

Release/discriminate.o	:	discriminate.cc
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o$@ $<

Release/invremap.o	:	invremap.cc
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o$@ $<

clean_release	:
	rm -rf Release/*.o
	rm -rf $(RELEASE_TARGET)

clean	:
	$(MAKE) clean_debug
	$(MAKE) clean_release
