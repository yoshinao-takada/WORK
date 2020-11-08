add_library(base_z STATIC
    BL_snparrayIO.c BL_snparray.c BL_styz.c BL_2port.c BL_snparrayIO2.c)
install(TARGETS base_z DESTINATION lib)