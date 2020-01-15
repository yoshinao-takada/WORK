#include "base_l/BL_loggerbase.h"
#include "base_l/BL_futils.h"
#include "base_l/BL_containers.h"
#include "base_l/BL_locker.h"
#include "base_l/BL_diag.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#pragma region declare_logger_objects
// declare logger object as static objects private to BL_loggerbase.
typedef struct {
    FILE* pf;
    char* dirpath;
    char* filepath;
    HANDLE h; /* common to mutex handle */
    BL_linkable_t timer_list; /* anchor node of time measure items */
} BL_logger_t, *pBL_logger_t;

typedef const BL_logger_t *pcBL_logger_t;

static BL_logger_t loggers[BL_loggerID_end] =
{
    {
        NULL, NULL, NULL, NULL,
        { &loggers[0].timer_list, &loggers[0].timer_list }
    },
};
#pragma endregion
int BL_logger_open(const char* filepath, BL_loggerID_t id, HANDLE* ph)
{
    int err = ESUCCESS;
    pBL_logger_t logger = NULL;
    *ph = NULL;
    do {
        if ((int)id >= (int)BL_loggerID_end)
        { // invaild ID
            err = EINVAL;
            break;
        }

        logger = &loggers[id];
        if (logger->pf)
        { // log file has already been opened.
            err = EALREADY;
            break;
        }

        logger->filepath = strdup(filepath);
        logger->dirpath = BL_futils_copydirpath(filepath);
        logger->pf = fopen(filepath, (id == BL_logger_openmode_append) ? "a" : "w");
        if (NULL == (logger->pf))
        { // cannot open log file
            err = errno;
            break;
        }

    } while (0);
    if (err && (err != EALREADY))
    {
        if (logger->dirpath)
        {
            free((void*)(logger->dirpath));
            logger->dirpath = NULL;
        }
        if (logger->filepath)
        {
            free((void*)(logger->filepath));
            logger->filepath = NULL;
        }
        if (logger->pf)
        {
            fclose(logger->pf);
            logger->pf = NULL;
        }
    }
    return err;
}

int BL_logger_get(BL_loggerID_t id, HANDLE* ph)
{
    int err = ESUCCESS;
    pBL_logger_t logger = NULL;
    *ph = NULL;
    do {
        if ((int)id >= (int)BL_loggerID_end)
        { // invaild ID
            err = EINVAL;
            break;
        }

        logger = &loggers[id];
        *ph = logger->h;
    } while (false);
    return err; 
}

static int CloseLogger(int id)
{
    int err = ESUCCESS;
    do {
        if (loggers[id].pf)
        {
            fclose(loggers[id].pf);
            free(loggers[id].dirpath);
            free(loggers[id].filepath);
            loggers[id].h = NULL;
            BL_linkable_unlink_other_than_anchor(&loggers[id].timer_list);
        }
        else
        {
            err = EALREADY;
        }
    } while (false);
    return err;
}

static BL_loggerID_t FindLoggerByHandle(HANDLE h)
{
    int id = 0;
    for (id = 0; id < (int)BL_loggerID_end; id++)
    {
        if (h == loggers[id].h)
        {
            break;
        }
    }
    return (BL_loggerID_t)id;
}

int BL_logger_close(HANDLE h)
{
    int err = ESUCCESS;
    do {
        BL_loggerID_t id = FindLoggerByHandle(h);
        if (id == BL_loggerID_end)
        {
            err = EINVAL;
            break;
        }
        err = CloseLogger(id);
    } while (false);
    return err; 
}

int BL_logger_beginrecord(HANDLE h, FILE**ppf)
{
    int err = ESUCCESS;
    do {
        BL_loggerID_t id = FindLoggerByHandle(h);
        if (id == BL_loggerID_end)
        {
            err = EINVAL;
            break;
        }
        *ppf = loggers[id].pf;
        if (*ppf == NULL)
        {
            err = EINVAL;
            break;
        }
        err = BL_locker_lock(h);
    } while (false);
    return err;
}

static BL_loggerID_t FindLoggerByFILE(FILE** ppf)
{
    int id = 0;
    for (; id != BL_loggerID_end; id++)
    {
        if (*ppf == loggers[id].pf)
        {
            break;
        }
    }
    return (BL_loggerID_t)id;
}

int BL_logger_endrecord(FILE** ppf)
{
    int err = ESUCCESS;
    do {
        BL_loggerID_t id = FindLoggerByFILE(ppf);
        if (id == BL_loggerID_end)
        {
            err = EINVAL;
            break;
        }
        BL_locker_unlock(loggers[id].h);
    } while (false);
    return err;
}

static BL_linkable_declare_anchor(time_measure_items);

static void SetTimeItem(pBL_linkable_array_t node, const struct timespec* tsu, const struct timespec* tss )
{
    BL_ptr_t u32array = BL_linkable_array_begin(node);
    u32array._1u32[2] = (BL_1u32_t)(tsu->tv_sec);
    u32array._1u32[3] = (BL_1u32_t)(tsu->tv_nsec);
    u32array._1u32[4] = (BL_1u32_t)(tss->tv_sec);
    u32array._1u32[5] = (BL_1u32_t)(tss->tv_nsec);
}

/*!
\brief create a new time measure item
\param id [in] measure ID
\param tvu [in] beginning process user time
\param tvs [in] beginning process system (kernel) time
*/
static pBL_linkable_t NewTimeItem(BL_measureID_t id, const struct timespec* tsu, const struct timespec* tss)
{
    pBL_linkable_array_t p = BL_linkable_array_new(6, BL_1u32_t);
    if (p == NULL)
    {
        return NULL;
    }
    SetTimeItem(p, tsu, tss);
    return &(p->link);
}

static bool MatchMeasureID(pcBL_linkable_t node, const void* pvID)
{
    BL_1u32_t u32ID = (BL_1u32_t)(uintptr_t)pvID;
    BL_cptr_t node_array = BL_linkable_array_cbegin((pcBL_linkable_array_t)node);
    return u32ID == node_array._1u32[0];
}

int BL_logger_begin_processtime(BL_measureID_t id)
{
    int err = ESUCCESS;
    do {
        struct timespec tsu, tss; // user time and system time
        err = BL_diag_process_time(&tsu, &tss);
        pBL_linkable_t time_measure_item = (pBL_linkable_t)
            BL_linkable_find(&time_measure_items, (const void*)id, MatchMeasureID);
        if (time_measure_item == NULL)
        {
            if (NULL == (time_measure_item = NewTimeItem(id, &tsu, &tss)))
            {
                errno = ENOMEM;
                break;
            }
            BL_linkable_link_next(&time_measure_items, time_measure_item);
        }
        else
        {
            SetTimeItem((pBL_linkable_array_t)time_measure_item, &tsu, &tss);
        }        
    } while (false);
    return err;
}

int BL_logger_get_processtime(BL_measureID_t id, struct timespec* tu, struct timespec* ts)
{
    int err = ESUCCESS;
    do {
        // find beginning of the time measurement item.
        pcBL_linkable_t time_measure_begin = BL_linkable_find(&time_measure_items, (const void*)id, MatchMeasureID);
        if (time_measure_begin == NULL)
        {
            err = EINVAL;
            break;
        }

        // get the beginning time of the time measurement
        struct timespec tsu_begin, tsu_end, tss_begin, tss_end;
        BL_cptr_t time_measure_begin_ = 
            BL_linkable_array_cbegin((pcBL_linkable_array_t)time_measure_begin);
        tsu_begin.tv_sec = time_measure_begin_._1u32[2];
        tsu_begin.tv_nsec = time_measure_begin_._1u32[3];
        tss_begin.tv_sec = time_measure_begin_._1u32[4];
        tss_begin.tv_nsec = time_measure_begin_._1u32[5];
        // get the time now.
        if (ESUCCESS != (err = BL_diag_process_time(&tsu_end, &tss_end)))
        {
            break;
        }
        // get the time differences
        if (ESUCCESS != (err = BL_logger_difftimespec(&tsu_end, &tsu_begin, tu)))
        {
            break;
        }
        if (ESUCCESS != (err = BL_logger_difftimespec(&tss_end, &tss_begin, ts)))
        {
            break;
        }
    } while (false);
    return err;
}

int BL_logger_delete_measure(BL_measureID_t id)
{
    return BL_linkable_delete(&time_measure_items, (const void*)id, MatchMeasureID);
}

#define ISO_DATETIME_FORMAT_LENGTH      20 /* including null termination */
int BL_logger_format_time(time_t t, char* buf)
{
    int err = ESUCCESS;
    do {
        struct tm tm;
        if (NULL == localtime_r(&t, &tm))
        {
            err = errno;
            break;
        }
        snprintf(buf, ISO_DATETIME_FORMAT_LENGTH, "%04d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    } while (false);
    return err;
}

#define I32_1E9     1000000000L

int BL_logger_difftimespec(
    const struct timespec* tbegin, const struct timespec* tend, struct timespec* tend_minus_tbegin)
{
    int err = ESUCCESS;
    do {
        if ((tend->tv_sec < tbegin->tv_sec) ||
            ((tend->tv_sec == tbegin->tv_sec) && (tend->tv_nsec < tbegin->tv_nsec)))
        {
            err = ERANGE;
            break;
        }
        tend_minus_tbegin->tv_sec = tend->tv_sec - tbegin->tv_sec;
        int32_t i_tv_nsec = (int32_t)(tend->tv_nsec) - (int32_t)(tbegin->tv_nsec);
        if (i_tv_nsec < 0)
        {
            i_tv_nsec += I32_1E9;
            tend_minus_tbegin->tv_sec--;
        }
        tend_minus_tbegin->tv_nsec = i_tv_nsec;
    } while (false);
    return err;
}

