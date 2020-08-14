add_library(base_l STATIC
    BL_diag.c BL_fsm.c BL_futils.c BL_locker.c BL_loggerbase.c BL_rational.c BL_rationalfunc.c
    BL_putils.c BL_debutil.c BL_containers.c)
target_compile_options(base_l
	PRIVATE	-Wno-unknown-pragmas)
