#ifndef BL_LOCKER_H_
#define BL_LOCKER_H_
#include "base_l/BL_base.h"
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum {
        BL_lockerID0, /* reserved for loggerID0 */
        BL_lockerID1,
        BL_lockerID2,
        BL_lockerID3,
        BL_lockerID_end /* don't use this. it is equal to the number of the available locker objects */
    } BL_lockerID_t;

    /*!
    \brief create a locker object.
    \param id [in] locker ID
    \param ph [out] locker handle
    \return unix errno compatible error code
    */
    int BL_locker_get(BL_lockerID_t id, HANDLE* ph);

    /*!
    \brief require lock and return immediately
    \param h [in] handle
    \return EBUSY: not available, ESUCCESS: available
    */
    int BL_locker_try_lock(HANDLE h);

    /*!
    \brief require lock, waiting until being available
    \param h [in] handle
    */
    int BL_locker_lock(HANDLE h);

    /*!
    \brief unlock
    \param h [in] handle
    */
    void BL_locker_unlock(HANDLE h);
#ifdef __cplusplus
}
#endif
#endif