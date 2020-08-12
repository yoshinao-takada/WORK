#ifndef TEST_DEFS_H_
#define TEST_DEFS_H_
#include    <gtest/gtest.h>
#include    <base_l/BL_base.h>
#include    <base_l/BL_errno.h>
#define HOME_DIR    "/home/yoshinao/"
#define PROJECT_DIR HOME_DIR "REPOS/WORK/"
#ifndef DATA_DIR
#define DATA_DIR    PROJECT_DIR "UTbase_l/"
#endif
#ifndef WORK_DIR
#define WORK_DIR    PROJECT_DIR "UTbase_l_work/"
#endif
#ifndef RESULT_DIR
#define RESULT_DIR  PROJECT_DIR "UTbase_l_result/"
#endif
#endif /* TEST_DEFS_H_ */
