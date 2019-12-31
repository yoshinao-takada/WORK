#include "base_l/BL_diag.h"
#include "base_l/BL_base.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

/*!
\brief get realtime clock value as struct timespec
*/
int BL_diag_realtime(struct timespec* time)
{
    int err = ESUCCESS;
    do {
        if (-1 == clock_gettime(CLOCK_REALTIME, time))
        {
            err = errno;
        }
    } while (false);
    return err;
}

/*!
\brief get process user time as struct timespec
*/
int BL_diag_process_user_time(struct timespec* time)
{
    int err = ESUCCESS;
    do {
        struct rusage r;
        if (-1 == getrusage(RUSAGE_SELF, &r))
        {
            err = errno;
        }
        time->tv_sec = r.ru_utime.tv_sec;
        time->tv_nsec = r.ru_utime.tv_usec * 1000;
    } while (false);
    return err;
}

/*!
\brief get thread user time as struct timespec
*/
int BL_diag_thread_user_time(struct timespec* time)
{
    int err = EINVAL; // WSL does not support thread time.
    do {
    } while (false);
    return err;
}

/*!
\brief get working set and peak working set
*/
int BL_diag_working_set(uint32_t* current, uint32_t* peak)
{
    static const char* headers[] = { "VmSize:", "VmPeak:" };
    static const int header_lengths[] = { sizeof(headers[0]), sizeof(headers[1]) };
    int err = ESUCCESS;
    FILE *pf = NULL;
    char str_buf[128];
    do {
        pid_t pid = getpid();
        snprintf(str_buf, sizeof(str_buf)-1, "/proc/%u/status", (unsigned int)pid);
        pf = fopen(str_buf, "r");
        if (pf == NULL)
        {
            err = errno;
            break;
        }
        for (int i = 0; i < 2; )
        {
            char* p = fgets(str_buf, ARRAYSIZE(str_buf), pf);
            if (p == NULL)
            {
                err = EINVAL;
                break;
            }
            if (strncmp(str_buf, headers[0], header_lengths[0]) == 0)
            {
                sscanf(str_buf + header_lengths[0], "%u", current);
                *current *= 1024;
                i++;
            }
            if (strncmp(str_buf, headers[1], header_lengths[1]) == 0)
            {
                sscanf(str_buf + header_lengths[1], "%u", peak);
                *peak *= 1024;
                i++;
            }
        }
    } while (false);
    return err;
}

void* BL_diag_timeout(void* params)
{
    pBL_timeout_t timeout = (pBL_timeout_t)params;
    if (timeout->status == BL_timeout_after || timeout->timeout.tv_nsec >= 1000000000)
    {
        errno = EINVAL;
        return NULL;
    }
    struct timespec wait_time = { timeout->timeout.tv_sec, timeout->timeout.tv_nsec };
    struct timespec remaining;
    int err = nanosleep(&wait_time, &remaining);
    timeout->status = BL_timeout_after;
    return params;
}