#ifndef BL_LOGGERBASE_H_
#define BL_LOGGERBASE_H_
#include "base_l/BL_base.h"
#include <time.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif
#pragma region App_programmer_configurable_items
    //---------------------------------------------
    // beginning of configurable items
    //---------------------------------------------

    /*!
    This configuration allows only a single logger.
    If you need multiple loggers, increase the number of BL_loggerID_t membes.
    */
    typedef enum {
        BL_loggerID0,
        BL_loggerID_end
    } BL_loggerID_t;

    /*!
    This configures time and memory measurement items.
    */
    typedef enum {
        BL_measureID_process_begin_end,
        BL_measureID_timespan0,
        BL_measureID_memory0,
    } BL_measureID_t;

#define BL_MEASURE_LABELS = { \
    "Process begin to end" \
    , "Time span 0" \
    , "Memory 0" \
}

    //---------------------------------------------
    // end of configurable items
    //---------------------------------------------
#pragma endregion

    typedef enum {
        BL_logger_openmode_overwrite,
        BL_logger_openmode_append,
    } BL_logger_openmode_t;

    /*!
    \brief open logger, getting a logger handle.
    */
    int BL_logger_open(const char* filepath, BL_loggerID_t id, HANDLE* ph);

    /*!
    \brief get a logger handle
    */
    int BL_logger_get(BL_loggerID_t id, HANDLE* ph);

    /*!
    \brief close logger
    */
    int BL_logger_close(HANDLE h);

    /*!
    \brief begin writing a record, lock the logger, get a FILE pointer.
    \param h [in] logger handle
    \param 
    */
    int BL_logger_beginrecord(HANDLE h, FILE**ppf);

    /*!
    \brief end writing a record, unlock the logger
    \param ppf [in,out] file stream obtained by calling BL_logger_beginrecord()
    */
    int BL_logger_endrecord(FILE** ppf);

    /*!
    \brief begin process time measurement
    \param id [in] measure ID
    */
    int BL_logger_begin_processtime(BL_measureID_t id);

    /*!
    \brief get a process time from its measurement beginning.
    \param id [in] measure ID
    \param tu [out] user time
    \param ts [out] system time
    */
    int BL_logger_get_processtime(BL_measureID_t id, struct timespec* tu, struct timespec* ts);

    /*!
    \brief delete a measurement item. The released id is available again for another use.
    \param id [in] measure ID
    */
    int BL_logger_delete_measure(BL_measureID_t id);

    /*!
    \brief format universal time in ISO format in local time (YYYY-MM-DD hh:mm:ss)
    */
    int BL_logger_format_time(time_t t, char* buf);

    /*!
    \brief calculate difference of two timespec instances
    \param tbegin [in]
    \param tend [in]
    \param tend_minus_tbegin [out]
    */
    int BL_logger_difftimespec(const struct timespec* tbegin, const struct timespec* tend, struct timespec* tend_minus_tbegin);
#ifdef __cplusplus
}
#endif
#endif /* BL_LOGGERBASE_H_ */