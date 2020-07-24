INCFLAGS = -I..
OCVINCFLAGS = -I/usr/include/opencv4
DEBUG_CCFLAGS = $(INCFLAGS) -c -g -DDEBUG_DATA_DIR="\"../UTData/\""
DEBUG_TARGET_DIR = ../Debug
DEBUG_LIBDIR = -L$(DEBUG_TARGET_DIR)
DEBUG_LIBS = -lbase_l -lm
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_g.a
DEBUG_OBJECTS = Debug/BL_matr32RM.o Debug/BL_matr64RM.o Debug/BL_matr32CM.o Debug/BL_matr64CM.o \
	Debug/BL_mesh.o Debug/BL_texture.o Debug/BL_distortion.o Debug/BL_bilinearinterp.o \
	Debug/BL_discriminate.o Debug/BL_twolines.o Debug/BL_intersect.o Debug/BL_plate.o \
	Debug/BL_3Dcylinder.o Debug/BL_3Dplane.o Debug/BL_3Dline.o
RELEASE_CCFLAGS = $(INCFLAGS) -c -O2
RELEASE_TARGET_DIR = ../Release
RELEASE_LIBDIR = -L$(RELEASE_TARGET_DIR)
RELEASE_LIBS = -lbase_l -lm
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_g.a
RELEASE_OBJECTS = Release/BL_matr32RM.o Release/BL_matr64RM.o Release/BL_matr32CM.o Release/BL_matr64CM.o \
	Release/BL_mesh.o Release/BL_texture.o Release/BL_distortion.o Release/BL_bilinearinterp.o \
	Release/BL_discriminate.o Release/BL_twolines.o Release/BL_intersect.o Release/BL_plate.o \
	Release/BL_3Dcylinder.o Release/BL_3Dplane.o Release/BL_3Dline.o
#-----------------------------------------------------------------------------------
# debug
#-----------------------------------------------------------------------------------

debug	:	$(DEBUG_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	rm -f $@
	ar -r $@ $^
	ranlib $@

Debug/BL_matr32RM.o	:	BL_mat.c catmacro.h globaldefr32RM.h localdefr32.h catmacro.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -DMATLAYOUTRM -DNUMTYPEr32 -o $@ $<

Debug/BL_matr64RM.o	:	BL_mat.c catmacro.h globaldefr64RM.h localdefr64.h catmacro.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -DMATLAYOUTRM -DNUMTYPEr64 -o $@ $<

Debug/BL_matr32CM.o	:	BL_mat.c catmacro.h globaldefr32CM.h localdefr32.h catmacro.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -DMATLAYOUTCM -DNUMTYPEr32 -o $@ $<

Debug/BL_matr64CM.o	:	BL_mat.c catmacro.h globaldefr64CM.h localdefr64.h catmacro.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -DMATLAYOUTCM -DNUMTYPEr64 -o $@ $<

Debug/BL_mesh.o	:	BL_mesh.c BL_mesh.h
	mkdir -p Debug
	$(CC) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_texture.o	:	BL_texture.cc BL_texture.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_distortion.o	:	BL_distortion.c BL_distortion.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_bilinearinterp.o	:	BL_bilinearinterp.c BL_bilinearinterp.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_discriminate.o	:	BL_discriminate.c BL_discriminate.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_twolines.o	:	BL_twolines.c BL_twolines.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_intersect.o	:	BL_intersect.c BL_intersect.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_plate.o	:	BL_plate.c BL_plate.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_3Dcylinder.o	:	BL_3Dcylinder.c BL_3Dcylinder.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_3Dplane.o	:	BL_3Dplane.c BL_3Dplane.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

Debug/BL_3Dline.o	:	BL_3Dline.c BL_3Dline.h
	mkdir -p Debug
	$(CC) $(OCVINCFLAGS) $(DEBUG_CCFLAGS) -o $@ $<

clean_debug	:
	rm -f Debug/*.o
	rm -f $(DEBUG_TARGET)

#-----------------------------------------------------------------------------------
# release
#-----------------------------------------------------------------------------------

release	:	$(RELEASE_TARGET)

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	rm -f $@
	ar -r $@ $^
	ranlib $@

Release/BL_matr32RM.o	:	BL_mat.c catmacro.h globaldefr32RM.h localdefr32.h catmacro.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -DMATLAYOUTRM -DNUMTYPEr32 -o $@ $<

Release/BL_matr64RM.o	:	BL_mat.c catmacro.h globaldefr64RM.h localdefr64.h catmacro.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -DMATLAYOUTRM -DNUMTYPEr64 -o $@ $<

Release/BL_matr32CM.o	:	BL_mat.c catmacro.h globaldefr32CM.h localdefr32.h catmacro.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -DMATLAYOUTCM -DNUMTYPEr32 -o $@ $<

Release/BL_matr64CM.o	:	BL_mat.c catmacro.h globaldefr64CM.h localdefr64.h catmacro.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -DMATLAYOUTCM -DNUMTYPEr64 -o $@ $<

Release/BL_mesh.o	:	BL_mesh.c BL_mesh.h
	mkdir -p Release
	$(CC) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_texture.o	:	BL_texture.cc BL_texture.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_distortion.o	:	BL_distortion.c BL_distortion.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_bilinearinterp.o	:	BL_bilinearinterp.c BL_bilinearinterp.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_discriminate.o	:	BL_discriminate.c BL_discriminate.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_twolines.o	:	BL_twolines.c BL_twolines.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_intersect.o	:	BL_intersect.c BL_intersect.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_plate.o	:	BL_plate.c BL_plate.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_3Dcylinder.o	:	BL_3Dcylinder.c BL_3Dcylinder.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_3Dplane.o	:	BL_3Dplane.c BL_3Dplane.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<

Release/BL_3Dline.o	:	BL_3Dline.c BL_3Dline.h
	mkdir -p Release
	$(CC) $(OCVINCFLAGS) $(RELEASE_CCFLAGS) -o $@ $<
	
clean_release	:
	rm -f Release/*.o
	rm -f $(RELEASE_TARGET)
