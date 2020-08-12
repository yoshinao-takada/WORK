list(APPEND EXTRA_LIBS base_g base_l gtest gtest_main pthread opencv_imgcodecs opencv_imgproc opencv_core)
add_executable(UTbase_g
    mat.cc interp.cc discriminate.cc distortion.cc invremap.cc
)
target_link_libraries(UTbase_g PUBLIC ${EXTRA_LIBS})
target_compile_options(UTbase_g PRIVATE
    -DWORK_DIR=\"./\"
    -DRESULT_DIR=\"../RESULTS/\"
)