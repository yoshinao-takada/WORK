WORK_DIR = ..
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/Debug/lib
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/Release/lib
DEBUG_TARGET = $(WORK_DIR)/Debug/UT.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UT.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH)
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) -lbase_l -lgtestd -lgtest_maind -lstdc++ -lpthread
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) -lbase_l -lgtest -lgtest_main -lstdc++ -lpthread
INCFLAGS = -I../gtest/include -I..
CC = gcc

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

DEBUG_OBJECTS = Debug/containers.o Debug/putils.o Debug/diag.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $^ $(DEBUG_LIB_FLAGS)

Debug/containers.o	:	containers.cc
	mkdir -p Debug
	$(CC) -c $(INCFLAGS) -o $@ $<

Debug/putils.o	:	putils.cc
	mkdir -p Debug
	$(CC) -c $(INCFLAGS) -o $@ $<

Debug/diag.o	:	diag.cc
	mkdir -p Debug
	$(CC) -c $(INCFLAGS) -o $@ $<

clean	:
	rm -f Debug/*.o
	rm -f Release/*.o
	rm -f ../Debug/*.exe
	rm -f ../Release/*.exe