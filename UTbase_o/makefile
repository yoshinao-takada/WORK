WORK_DIR = ..
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/lib
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/lib
OCV_INCFLAGS = -I/usr/include/opencv4
DEBUG_CC_FLAGS = $(OCV_INCFLAGS) -c -g -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\"" -D_DEBUG
RELEASE_CC_FLAGS = $(OCV_INCFLAGS) -c -O2 -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\"" -DNDEBUG
DEBUG_TARGET = $(WORK_DIR)/Debug/UTbase_o.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UTbase_o.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH) -L/usr/lib64
OCV_LIB_FLAGS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) -lgtestd -lgtest_maind -lbase_o -lbase_g -lbase_l $(OCV_LIB_FLAGS) -lstdc++ -lpthread -lm
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) -lgtest -lgtest_main -lbase_o -lbase_g -lbase_l $(OCV_LIB_FLAGS) -lstdc++ -lpthread -lm
INCFLAGS = -I../gtest/include -I..
CC = cc

#---------------------------------------------------
# debug
#---------------------------------------------------
debug	:	$(DEBUG_TARGET)

DEBUG_OBJECTS = Debug/linesearch.o Debug/nelder1.o Debug/dfo.o Debug/optc.o Debug/penalty0.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIB_FLAGS)

Debug/linesearch.o	:	linesearch.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/nelder1.o	:	nelder1.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/dfo.o	:	dfo.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/optc.o	:	optc.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/penalty0.o	:	penalty0.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_debug	:
	rm -f Debug/*.o
	rm -f ../Debug/*.exe

#---------------------------------------------------
# debug
#---------------------------------------------------
release	:	$(RELEASE_TARGET)

RELEASE_OBJECTS = Release/linesearch.o Release/nelder1.o Release/dfo.o Release/optc.o Release/penalty0.o

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(WORK_DIR)/Release
	$(CC) -o $@ $(RELEASE_OBJECTS) $(RELEASE_LIB_FLAGS)

Release/linesearch.o	:	linesearch.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/nelder1.o	:	nelder1.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/dfo.o	:	dfo.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/optc.o	:	optc.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/penalty0.o	:	penalty0.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_release	:
	rm -f Release/*.o
	rm -f ../Release/*.exe
