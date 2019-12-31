#include "gtest/gtest.h"
#include "base_l/BL_base.h"
#include "base_l/BL_diag.h"
#include <pthread.h>

#define SUIT    diag

namespace
{
    /*!
    \brief measure elapsed real time and user CPU time.
    */
    TEST(SUIT, elapsed_realtime_utime)
    {
        BL_timeout_t timeout = { { 5, 500000000 }, BL_timeout_before };
        pthread_t thread;
        pthread_attr_t attr;
        int err = pthread_attr_init(&attr);
        ASSERT_EQ(ESUCCESS, err);
        struct timespec realtime_begin, usertime_begin;
        err = BL_diag_realtime(&realtime_begin);
        ASSERT_EQ(ESUCCESS, err);
        err = BL_diag_process_user_time(&usertime_begin);
        ASSERT_EQ(ESUCCESS, err);
        err = pthread_create(&thread, &attr, BL_diag_timeout, (void*)&timeout);
        ASSERT_EQ(ESUCCESS, err);
        while (timeout.status == BL_timeout_before) {;}
        struct timespec realtime_end, usertime_end;
        err = BL_diag_realtime(&realtime_end);
        ASSERT_EQ(ESUCCESS, err);
        err = BL_diag_process_user_time(&usertime_end);
        ASSERT_EQ(ESUCCESS, err);
        struct timespec realtime_elapsed = BL_timespec_subtract(&realtime_end, &realtime_begin);
        struct timespec usertime_elapsed = BL_timespec_subtract(&usertime_end, &usertime_begin);
        double timeout_refd = BL_timespec_to_double(&timeout.timeout);
        double realtime_elapsedd = BL_timespec_to_double(&realtime_elapsed);
        double usertime_elapsedd = BL_timespec_to_double(&usertime_elapsed);
        double timeout_told = 0.3;
        ASSERT_EQ(true, BL_diag_nearly_equald(timeout_refd, realtime_elapsedd, timeout_told));
        ASSERT_EQ(true, BL_diag_nearly_equald(timeout_refd, usertime_elapsedd, timeout_told));
    }
}