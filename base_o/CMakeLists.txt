add_library(base_l "")

target_source(base_l
PRIVATE
    BL_putils.h BL_putils.c
    BL_diag.h BL_diag.c
    BL_futils.h BL_futils.c
    BL_locker.h BL_locker.c
)

target_include_directories(base_l
PRIVATE "${PROJECT_SOURCE_DIR}/.."
)
