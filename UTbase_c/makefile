WORK_DIR = ..
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/lib
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/lib
DEBUG_CC_FLAGS = -c -g -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
RELEASE_CC_FLAGS = -c -O2 -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
DEBUG_TARGET = $(WORK_DIR)/Debug/UTbase_c.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UTbase_c.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH)
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) -lbase_c -lbase_l -lgtestd -lgtest_maind -lstdc++ -lpthread
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) -lbase_c -lbase_l -lgtest -lgtest_main -lstdc++ -lpthread
INCFLAGS = -I../gtest/include -I..
CC = cc

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

DEBUG_OBJECTS = Debug/crc.o Debug/ringbuffer.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIB_FLAGS)

Debug/crc.o	:	crc.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/ringbuffer.o	:	ringbuffer.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_debug	:
	rm -f Debug/*.o
	rm -f $(DEBUG_TARGET)

clean_release	:
	rm -f Release/*.o
	rm -f $(RELEASE_TARGET)
