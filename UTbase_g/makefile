WORK_DIR = ..
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/lib
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/lib
OCV_INCFLAGS = -I/usr/include/opencv4
DEBUG_CC_FLAGS = $(OCV_INCFLAGS) -c -g -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
RELEASE_CC_FLAGS = $(OCV_INCFLAGS) -c -O2 -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
DEBUG_TARGET = $(WORK_DIR)/Debug/UTbase_g.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UTbase_g.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH) -L/usr/lib64
OCV_LIB_FLAGS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) -lbase_g -lbase_l -lgtestd -lgtest_maind $(OCV_LIB_FLAGS) -lstdc++ -lpthread -lm
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) -lbase_g -lbase_l -lgtest -lgtest_main $(OCV_LIB_FLAGS) -lstdc++ -lpthread -lm
INCFLAGS = -I../gtest/include -I..
CC = cc

#---------------------------------------------------
# debug
#---------------------------------------------------
debug	:	$(DEBUG_TARGET)

DEBUG_OBJECTS = Debug/mat.o Debug/texture.o Debug/mesh.o Debug/distortion.o Debug/interp.o \
	Debug/discriminate.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIB_FLAGS)

Debug/mat.o	:	mat.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/texture.o	:	texture.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/mesh.o	:	mesh.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/distortion.o	:	distortion.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/interp.o	:	interp.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/discriminate.o	:	discriminate.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_debug	:
	rm -f Debug/*.o
	rm -f ../Debug/*.exe

#---------------------------------------------------
# debug
#---------------------------------------------------
release	:	$(RELEASE_TARGET)

RELEASE_OBJECTS = Release/mat.o Release/texture.o Release/mesh.o Release/distortion.o Release/interp.o \
	Release/discriminate.o

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(WORK_DIR)/Release
	$(CC) -o $@ $(RELEASE_OBJECTS) $(RELEASE_LIB_FLAGS)

Release/mat.o	:	mat.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/texture.o	:	texture.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/mesh.o	:	mesh.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/distortion.o	:	distortion.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/interp.o	:	interp.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/discriminate.o	:	discriminate.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_release	:
	rm -f Release/*.o
	rm -f ../Release/*.exe
