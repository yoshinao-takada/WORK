#ifndef BL_BASE_H_
#define BL_BASE_H_
#ifndef ARRAYSIZE
#define ARRAYSIZE(a)    (sizeof(a)/sizeof(a[0]))
#endif

#ifndef __max
#define __max(a,b)  ((a > b) ? a : b)
#endif
#ifndef __min
#define __min(a,b)  ((a < b) ? a : b)
#endif

#ifndef _WIN32
typedef void*   HANDLE;
#endif
#endif