add_library(base_o STATIC
    BL_linesearch.c BL_optimize.c BL_steepestdescent.c BL_cg.c BL_constraint.c BL_nelder1.c BL_dfo.c
    BL_penalty.c
    )
install(TARGETS base_o DESTINATION lib)