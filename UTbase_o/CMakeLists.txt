list(APPEND EXTRA_LIBS base_o base_g base_l gtest gtest_main pthread opencv_imgcodecs opencv_imgproc opencv_core)
add_executable(UTbase_o
    linesearch.cc nelder1.cc dfo.cc optc.cc penalty0.cc
)
target_link_libraries(UTbase_o PUBLIC ${EXTRA_LIBS})
target_compile_options(UTbase_o PRIVATE
    -DWORK_DIR=\"./\"
    -DRESULT_DIR=\"../RESULTS/\"
)