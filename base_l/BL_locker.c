#include <pthread.h>
#include <errno.h>
#include "base_l/BL_locker.h"
    static pthread_mutex_t lockers[BL_lockerID_end] =
    {
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
        PTHREAD_MUTEX_INITIALIZER,
    };

    int BL_locker_get(BL_lockerID_t id, HANDLE* ph)
    {
        int err = ESUCCESS;
        if ((int)BL_lockerID_end > (int)id)
        {
            *ph = (HANDLE)&lockers[id];
        }
        else
        {
            err = EINVAL;
        }
        return err;
        
    }

    int BL_locker_try_lock(HANDLE h)
    {
        return pthread_mutex_trylock((pthread_mutex_t*)h);
    }

    int BL_locker_lock(HANDLE h)
    {
        return pthread_mutex_lock((pthread_mutex_t*)h);
    }

    void BL_locker_unlock(HANDLE h)
    {
        pthread_mutex_unlock((pthread_mutex_t*)h);
    }
