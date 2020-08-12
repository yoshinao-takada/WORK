list(APPEND EXTRA_LIBS base_l gtest gtest_main pthread)
add_executable(UTbase_l
    containers.cc rational.cc diag.cc futils.cc putils.cc
)
target_link_libraries(UTbase_l PUBLIC ${EXTRA_LIBS})
target_compile_options(UTbase_l PRIVATE
    -DWORK_DIR=\"./\"
    -DRESULT_DIR=\"../RESULTS/\"
)