list(APPEND EXTRA_LIBS base_z base_g base_l gtest gtest_main pthread)
add_executable(UTbase_z
    IO2.cc
)
target_link_libraries(UTbase_z PUBLIC ${EXTRA_LIBS})
target_compile_options(UTbase_z PRIVATE
    -DWORK_DIR=\"./\"
    -DRESULT_DIR=\"${CMAKE_CURRENT_LIST_DIR}/../../RESULTS/\"
)