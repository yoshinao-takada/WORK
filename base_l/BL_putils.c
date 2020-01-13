#include "base_l/BL_putils.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <unistd.h>
#include <errno.h>

int BL_putils_execv_sync(char* const argv[], int* exit_code)
{
    int err = ESUCCESS;
    do {
        HANDLE h_process = NULL;
        if (ESUCCESS != (err = BL_putils_execv(argv, &h_process)))
        {
            break;
        }
        err = BL_putils_wait_process(h_process, exit_code);
    } while (false);
    return err;
}

int BL_putils_execv(char* const argv[], HANDLE* ph_proc)
{
    int err = ESUCCESS;
    do {
        pid_t   pid = fork();
        if (pid == 0) 
        { // child process
            if  (-1 == execv(argv[0], argv))
            {
                err = errno;
                break;
            }
        }
        else
        { // parent process
            *ph_proc = (HANDLE)(intptr_t)pid;
        }        
    } while (false);
    return err;
}


int BL_putils_wait_process(HANDLE h_proc, int* exit_code)
{
    int err = ESUCCESS;
    pid_t pid = (pid_t)(intptr_t)h_proc;
    do {
        int status = 0;
        if (-1 == waitid(P_PID, pid, NULL, WEXITED))
        {
            err = errno;
            break;
        }
        if (WIFEXITED(status))
        {
            *exit_code = WEXITSTATUS(status);
        }
        else
        {
            *exit_code = 0;
            err = EFAULT;
        }        
    } while (false);
    return err;
}

