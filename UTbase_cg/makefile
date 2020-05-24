WORK_DIR = ..
VCPKG_ROOT  = $(HOME)/REPOS/vcpkg/installed/x64-linux
GTEST_INCPATH = $(WORK_DIR)/gtest/include
GTEST_DEBUG_LIBPATH = $(WORK_DIR)/gtest/lib64
GTEST_RELEASE_LIBPATH = $(WORK_DIR)/gtest/lib64
DEBUG_CC_FLAGS = -c -g -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
RELEASE_CC_FLAGS = -c -O2 -DUTDATA="\"$(HOME)/REPOS/WORK/UTData/\""
DEBUG_TARGET = $(WORK_DIR)/Debug/UTbase_cg.exe
RELEASE_TARGET = $(WORK_DIR)/Release/UTbase_cg.exe
DEBUG_LIB_DIR = -L../Debug -L$(GTEST_DEBUG_LIBPATH)
DEBUG_OPENCV_LIBS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
DEBUG_OPENGL_LIBS = -lGL -lGLU -lxcb-glx -lX11
DEBUG_LIB_FLAGS = $(DEBUG_LIB_DIR) $(DEBUG_OPENCV_LIBS) -lbase_cg -lbase_l -lgtestd -lgtest_maind -lstdc++ -lpthread $(DEBUG_OPENGL_LIBS) -lm
RELEASE_LIB_DIR = -L../Release -L$(GTEST_RELEASE_LIBPATH)
RELEASE_OPENCV_LIBS = -lopencv_imgcodecs -lopencv_imgproc -lopencv_core
RELEASE_OPENGL_LIBS = -lGL -lGLU -lxcb-glx -lX11
RELEASE_LIB_FLAGS = $(RELEASE_LIB_DIR) $(RELEASE_OPENCV_LIBS) -lbase_cg -lbase_l -lgtest -lgtest_main -lstdc++ -lpthread $(RELEASE_OPENGL_LIBS) -lm
INCFLAGS = -I../gtest/include -I.. -I/usr/include/opencv4
CC = cc
DEBUG_OBJECTS = Debug/oomesh.o Debug/tristrip.o Debug/viewer0.o
RELEASE_OBJECTS = Release/oomesh.o Release/tristrip.o Release/viewer0.o

#----------------------------------------------------------
# debug build
#----------------------------------------------------------
debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(WORK_DIR)/Debug
	$(CC) -o $@ $(DEBUG_OBJECTS) $(DEBUG_LIB_FLAGS)

Debug/oomesh.o	:	oomesh.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/viewer0.o	:	viewer0.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?

Debug/tristrip.o	:	tristrip.cc
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ $?


#----------------------------------------------------------
# release build
#----------------------------------------------------------
release	:	$(RELEASE_TARGET)

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(WORK_DIR)/Release
	$(CC) -o $@ $(RELEASE_OBJECTS) $(RELEASE_LIB_FLAGS)

Release/oomesh.o	:	oomesh.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/viewer0.o	:	viewer0.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

Release/tristrip.o	:	tristrip.cc
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ $?

#----------------------------------------------------------
# debug clean
#----------------------------------------------------------
clean_debug	:
	rm -f Debug/*.o
	rm -f $(DEBUG_TARGET)
#----------------------------------------------------------
# release clean
#----------------------------------------------------------
clean_release	:
	rm -f Release/*.o
	rm -f $(RELEASE_TARGET)
