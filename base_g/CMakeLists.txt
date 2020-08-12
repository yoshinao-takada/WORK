add_compile_options(-Wno-unknown-pragmas)
add_library(BL_matr32RM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matr32RM.o PRIVATE MATLAYOUTRM NUMTYPEr32)
add_library(BL_matr64RM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matr64RM.o PRIVATE MATLAYOUTRM NUMTYPEr64)
add_library(BL_matc64RM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matc64RM.o PRIVATE MATLAYOUTRM NUMTYPEc64)
add_library(BL_matc128RM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matc128RM.o PRIVATE MATLAYOUTRM NUMTYPEc128)

add_library(BL_matr32CM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matr32CM.o PRIVATE MATLAYOUTCM NUMTYPEr32)
add_library(BL_matr64CM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matr64CM.o PRIVATE MATLAYOUTCM NUMTYPEr64)
add_library(BL_matc64CM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matc64CM.o PRIVATE MATLAYOUTRM NUMTYPEc64)
add_library(BL_matc128CM.o OBJECT BL_mat.c)
target_compile_definitions(BL_matc128CM.o PRIVATE MATLAYOUTRM NUMTYPEc128)

add_library(base_g STATIC
    $<TARGET_OBJECTS:BL_matr32RM.o>
    $<TARGET_OBJECTS:BL_matr64RM.o>
    $<TARGET_OBJECTS:BL_matc64RM.o>
    $<TARGET_OBJECTS:BL_matc128RM.o>
    $<TARGET_OBJECTS:BL_matr32CM.o>
    $<TARGET_OBJECTS:BL_matr64CM.o>
    $<TARGET_OBJECTS:BL_matc64CM.o>
    $<TARGET_OBJECTS:BL_matc128CM.o>
    BL_mesh.c
    BL_texture.cc
    BL_distortion.c
    BL_bilinearinterp.c
    BL_discriminate.c
    BL_twolines.c
    BL_intersect.c
    BL_plate.c
    BL_3Dcylinder.c
    BL_3Dplane.c
    BL_3Dline.c
    BL_invremap.c
    BL_invremap_testgen.c
    )