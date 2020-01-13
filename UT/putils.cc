#include "gtest/gtest.h"
#include "base_l/BL_base.h"
#include "base_l/BL_putils.h"
#include "base_l/BL_diag.h"
#include "base_l/BL_futils.h"

#define SUIT    putils

namespace
{
    /*!
    create a new process and wait for its termination.
    */
    TEST(SUIT, create_and_wait)
    {
        double time_tol_d = 0.5;
        // clock_gettime() to record the start time
        struct timespec time_begin, time_end;
        int err = BL_diag_realtime(&time_begin);

        // invoke "sleep 10" as a new process.
        char cmd_path[] = "/usr/bin/sleep";
        char cmd_arg[] = "5";
        char* const args[] = { cmd_path, cmd_arg, NULL };
        HANDLE h;
        err = BL_putils_execv(args, &h);

        // wait for the process termination
        int exit_code;
        err = BL_putils_wait_process(h, &exit_code);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_EQ(ESUCCESS, exit_code);

        // clock_gettime() to record the end time
        err = BL_diag_realtime(&time_end);
        ASSERT_EQ(ESUCCESS, err);

        // check the elapsed time
        struct timespec elapsed = BL_timespec_subtract(&time_end, &time_begin);
        double elapsed_d = BL_timespec_to_double(&elapsed);
        ASSERT_EQ(true, BL_diag_nearly_equald(5.0, elapsed_d, time_tol_d));
    }

    /*!
    execute a child process synchronously.
    */
#define DATA_DIR    "/home/yoshinao/REPOS/WORK_A0/UTData/"
#define DATA_DIR_OUT    DATA_DIR "Out/"
#define TOUCH_FILE      DATA_DIR_OUT "touch_file.txt"

    TEST(SUIT, exec_sync)
    {
        int err = BL_futils_create_directories(DATA_DIR_OUT);
        ASSERT_EQ(ESUCCESS, err);
        // execute touch
        {
            char cmd_path[] = "/usr/bin/touch";
            char cmd_arg[] = TOUCH_FILE;
            char* const args[] = { cmd_path, cmd_arg, NULL };
            int exit_code = 0;
            err = BL_putils_execv_sync(args, &exit_code);
            ASSERT_EQ(ESUCCESS, err);
            ASSERT_EQ(ESUCCESS, exit_code);
        }

        // check file existence
        bool b_file_exists = BL_futils_exists(TOUCH_FILE);
        ASSERT_EQ(true, b_file_exists);

        // execute rm
        {
            char cmd_path[] = "/usr/bin/rm";
            char cmd_arg[] = TOUCH_FILE;
            char* const args[] = { cmd_path, cmd_arg, NULL };
            int exit_code = 0;
            err = BL_putils_execv_sync(args, &exit_code);
            ASSERT_EQ(ESUCCESS, err);
            ASSERT_EQ(ESUCCESS, exit_code);
        }

        // check file existence
        b_file_exists = BL_futils_exists(TOUCH_FILE);
        ASSERT_EQ(false, b_file_exists);
    }
}