WORK_DIR = ..
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/lib
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/lib
DEBUG_CC_FLAGS = -c -g -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
RELEASE_CC_FLAGS = -c -O2
DEBUG_TARGET = $(WORK_DIR)/Debug/UT.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UT.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH)
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) -lbase_l -lgtestd -lgtest_maind -lstdc++ -lpthread
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) -lbase_l -lgtest -lgtest_main -lstdc++ -lpthread
INCFLAGS = -I../gtest/include -I..
DEF_DATA_DIR = -DDATA_DIR="\"../UTDATA_base_l/\""

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

DEBUG_OBJECTS = Debug/containers.o Debug/putils.o Debug/diag.o Debug/futils.o \
	Debug/json2.o Debug/rationalfunc.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIB_FLAGS)

Debug/containers.o	:	containers.cc ../base_l/BL_containers.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $<

Debug/putils.o	:	putils.cc ../base_l/BL_putils.h ../base_l/BL_putils.c
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $<

Debug/diag.o	:	diag.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/futils.o	:	futils.cc ../base_l/BL_futils.h ../base_l/BL_futils.c
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $<

Debug/rationalfunc.o	:	rationalfunc.cc ../base_l/BL_rationalfunc.h ../base_l/BL_rationalfunc.c
	mkdir -p Debug
	$(CC) $(DEF_DATA_DIR) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $<

# Debug/json.o	:	json.cc
# 	mkdir -p Debug
# 	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/json2.o	:	json2.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

clean_debug	:
	rm -f Debug/*.o
	rm -f ../Debug/*.exe

clean_release	:
	rm -f Release/*.o
	rm -f ../Release/*.exe
