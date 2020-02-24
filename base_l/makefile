CC = cc
INCFLAGS = -I..
DEBUG_CC_FLAGS = -c -g
DEBUG_TARGET_DIR = ../Debug
DEBUG_TARGET = $(DEBUG_TARGET_DIR)/libbase_l.a
RELEASE_CC_FLAGS = -c -O2
RELEASE_TARGET_DIR = ../Release
RELEASE_TARGET = $(RELEASE_TARGET_DIR)/libbase_l.a
DEBUG_OBJECTS = Debug/BL_putils.o Debug/BL_diag.o Debug/BL_futils.o Debug/BL_locker.o \
	Debug/BL_loggerbase.o Debug/BL_fsm.o Debug/BL_serializebase.o
RELEASE_OBJECTS = Release/BL_putils.o Release/BL_diag.o Release/BL_futils.o Release/BL_locker.o \
	Release/BL_loggerbase.o Release/BL_fsm.o Release/BL_serializebase.o

debug	:	$(DEBUG_TARGET)

release	:	$(RELEASE_TARGET)

$(DEBUG_TARGET)	:	$(DEBUG_OBJECTS)
	mkdir -p $(DEBUG_TARGET_DIR)
	ar r $@ $(DEBUG_OBJECTS)
	ranlib $@

Debug/BL_putils.o	:	BL_putils.c	BL_putils.h BL_base.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_putils.c

Debug/BL_diag.o	:	BL_diag.c	BL_diag.h BL_base.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_diag.c

Debug/BL_futils.o	:	BL_futils.c	BL_futils.h BL_base.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_futils.c

Debug/BL_locker.o	:	BL_locker.c	BL_locker.h BL_base.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_locker.c

Debug/BL_loggerbase.o	:	BL_loggerbase.c	BL_loggerbase.h BL_base.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_loggerbase.c

Debug/BL_fsm.o	:	BL_fsm.c	BL_fsm.h
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_fsm.c

Debug/BL_serializebase.o	:	BL_serializebase.c
	mkdir -p Debug
	$(CC) $(DEBUG_CC_FLAGS) $(INCFLAGS) -o $@ BL_serializebase.c

$(RELEASE_TARGET)	:	$(RELEASE_OBJECTS)
	mkdir -p $(RELEASE_TARGET_DIR)
	ar r $@ $(RELEASE_OBJECTS)
	ranlib $@

Release/BL_putils.o	:	BL_putils.c BL_putils.h BL_base.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_putils.c

Release/BL_diag.o	:	BL_diag.c BL_diag.h BL_base.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_diag.c

Release/BL_futils.o	:	BL_futils.c BL_futils.h BL_base.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_futils.c

Release/BL_locker.o	:	BL_locker.c BL_locker.h BL_base.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_locker.c

Release/BL_loggerbase.o	:	BL_loggerbase.c BL_loggerbase.h BL_base.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_loggerbase.c

Release/BL_fsm.o	:	BL_fsm.c	BL_fsm.h
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_fsm.c

Release/BL_serializebase.o	:	BL_serializebase.c
	mkdir -p Release
	$(CC) $(RELEASE_CC_FLAGS) $(INCFLAGS) -o $@ BL_serializebase.c

clean	:
	rm -f Debug/*
	rm -f Release/*
	rm -f $(DEBUG_TARGET)
	rm -f $(RELEASE_TARGET)
