# WORK_DIR should be defined as a compile option like -DWORK_DIR=\"xxx/yyy/\"
# RESULT_DIR should be defined as a compile option like -DRESULT_DIR=\"xxx/yyy/\"
# DATA_DIR shold be defined as a compile option like -DDATA_DIR=\"aaa/bbb\"
DEF_DATA_DIR = -DWORK_DIR=\"./\" -DRESULT_DIR=\"../RESULTS/\" -DDATA_DIR=\"../DATA/UTbase_l/\"
GTEST_INCFLAGS = -I../gtest/include
GTEST_DEBUG_LIBDIR = -L../gtest/lib
GTEST_RELEASE_LIBDIR = -L../gtest/lib
INCFLAGS = -I../gtest/include -I..
DEBUG_CC_FLAGS = -c -g $(INCFLAGS)
RELEASE_CC_FLAGS = -c -O2 $(INCFLAGS)
DEBUG_TARGET_DIR = ../../Debug
RELEASE_TARGET_DIR = ../../Release
DEBUG_TRGET = $(DEBUG_TARGET_DIR)/UTbase_l
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/UTbase_l
DEBUG_LIB_DIR = -L$(DEBUG_TARGET_DIR)
RELEASE_LIB_DIR = -L$(RELEASE_TARGET_DIR)
DEBUG_LIBS = $(DEBUG_LIB_DIR) -lbase_l -lgtestd -lgtest_maind -lstdc++ -pthread
RELEASE_LIBS = $(RELEASE_LIB_DIR) -lbase_l -lgtest -lgtest_main -lstdc++ -pthread

#---------------------------------------------------------------
# debug rules
#---------------------------------------------------------------
debug	:	$(DEBUG_TARGET)

DEBUG_OBJECTS = Debug/containers.o Debug/putils.o Debug/diag.o

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIBS)

Debug/containers.o	:	containers.cc ../base_l/BL_containers.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

Debug/putils.o	:	putils.cc ../base_l/BL_putils.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

Debug/diag.o	:	diag.cc ../base_l/BL_diag.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -rf Debug/*.o
	rm -f $(DEBUG_TARGET)

#---------------------------------------------------------------
# release rules
#---------------------------------------------------------------
release	:	$(RELEASE_TARGET)

RELEASE_OBJECTS = Release/containers.o Release/putils.o Release/diag.o

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	$(CC) -o $@ $(RELEASE_OBJECTS) $(RELEASE_LIBS)

Release/containers.o	:	containers.cc ../base_l/BL_containers.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

Release/putils.o	:	putils.cc ../base_l/BL_putils.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

Release/diag.o	:	diag.cc ../base_l/BL_diag.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) -o $@ $<

clean_debug	:
	rm -rf Release/*.o
	rm -f $(RELEASE_TARGET)

