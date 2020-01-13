#ifndef BL_BASE_H_
#define BL_BASE_H_
#include <stdint.h>
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))
#endif

#ifndef __max
#define __max(a,b)  ((a > b) ? a : b)
#endif
#ifndef __min
#define __min(a,b)  ((a < b) ? a : b)
#endif

#define ESUCCESS    0

#ifndef _WIN32
typedef void*   HANDLE;
typedef uint32_t DWORD;
typedef uint64_t ULONGLONG;
typedef struct _FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
typedef union _ULARGE_INTEGER {
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    DWORD LowPart;
    DWORD HighPart;
  } u;
  ULONGLONG QuadPart;
} ULARGE_INTEGER;
#endif
#ifdef __linux__
#define CHR_DIRSEP  '/'
#define STR_DIRSEP  "/"
#elif defined(_WIN32)
#define CHR_DIRSEP  '\\'
#define STR_DIRSEP  "\\"
#endif
inline struct timespec BL_timespec_add(const struct timespec* ts0, const struct timespec* ts1)
{
    static const long int _1e9 = 1000000000L;
    struct timespec ts_sum = 
    { ts0->tv_sec + ts1->tv_sec, ts0->tv_nsec + ts1->tv_nsec };
    if (ts_sum.tv_nsec >= _1e9)
    {
        ts_sum.tv_sec++;
        ts_sum.tv_nsec -= _1e9;
    }
    return ts_sum;
}

inline struct timespec BL_timespec_subtract(const struct timespec* ts0, const struct timespec* ts1)
{
    static const long int _1e9 = 1000000000L;
    struct timespec ts_sub =
    { ts0->tv_sec - ts1->tv_sec, ts0->tv_nsec - ts1->tv_nsec };
    if (ts_sub.tv_nsec < 0)
    {
        ts_sub.tv_sec--;
        ts_sub.tv_nsec += _1e9;
    }
    else if (ts_sub.tv_nsec >= _1e9)
    {
        ts_sub.tv_sec++;
        ts_sub.tv_nsec -= _1e9;
    }
    return ts_sub;
}

inline double BL_timespec_to_double(const struct timespec* ts)
{
    return (double)(ts->tv_sec) + 1.0e-9 * (double)(ts->tv_nsec);
}

#ifdef __cplusplus
}
#endif
#endif /* BL_BASE_H_ */
