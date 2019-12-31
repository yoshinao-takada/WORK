#ifndef BL_DIAG_H_
#define BL_DIAG_H_
#include <time.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
\brief get realtime clock value as struct timespec
*/
int BL_diag_realtime(struct timespec* time);

/*!
\brief get process user time as struct timespec
*/
int BL_diag_process_user_time(struct timespec* time);

/*!
\brief get thread user time as struct timespec
*/
int BL_diag_thread_user_time(struct timespec* time);

/*!
\brief get working set and peak working set
*/
int BL_diag_working_set(uint32_t* current, uint32_t* peak);

/*!
These enumeration and struct specifies timeout parameters
*/
typedef enum {
    BL_timeout_before,
    BL_timeout_after
} BL_timeout_status_t;

typedef struct {
    struct timespec timeout;
    BL_timeout_status_t status;    
} BL_timeout_t, *pBL_timeout_t;

typedef const BL_timeout_t *pcBL_timeout_t;

/*!
\brief wait and set a flag. Wait for seconds specified by (pBL_timeout_t)params->timeout.
After the timeout, sets status to BL_timeout_after.
\param params [in,out] (void*) casted BL_timeout_t struct.
\return the pointer to the BL_timeout_t struct.
*/
void* BL_diag_timeout(void* params);

inline bool BL_diag_nearly_equald(double a, double b, double tol)
{
    tol = fabs(tol);
    double lower_limit = a - tol;
    double upper_limit = a + tol;
    return (lower_limit < b) && (b < upper_limit);
}

inline bool BL_diag_nearly_equalf(float a, float b, float tol)
{
    tol = fabs(tol);
    float lower_limit = a - tol;
    float upper_limit = a + tol;
    return (lower_limit < b) && (b < upper_limit);
}

#ifdef __cplusplus
}
#endif
#endif