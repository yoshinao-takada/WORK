#include "base_l/BL_futils.h"
#include "base_l/BL_base.h"
#include <stdlib.h>
#include <errno.h>
void BL_futils_split(char* pathstr)
{
    while (*pathstr)
    {
        if (*pathstr == CHR_DIRSEP)
        {
            *pathstr = '\0';
        }
        pathstr++;
    }
}

const char* BL_futils_findfilename(const char* filepath)
{
    const char* filename = filepath;
    const char* scanner = filepath + strlen(filepath) - 1;
    const char* scan_end = filepath - 1;
    for (; scanner != scan_end; scanner--)
    {
        if (*scanner == CHR_DIRSEP)
        {
            filename = scanner + 1;
            break;
        }
    }
    return filename;
}

char* BL_futils_copydirpath(const char* filepath)
{
    const char* filename = BL_futils_findfilename(filepath);
    char* path_buffer = (char*)calloc(filename - filepath + 1, 1);
    strncpy(path_buffer, filepath, filename - filepath);
    return path_buffer;
}

char* BL_futils_append(const char* path_left, const char* path_right)
{
    char* path_buffer = NULL;
    int cb_alloc = 0;
    const char* format_string = NULL;
    if (*(path_left + strlen(path_left) - 1) == CHR_DIRSEP)
    {
        cb_alloc = strlen(path_left) + strlen(path_right) + 1;
        format_string = "%s%s";
    }
    else
    {
        cb_alloc = strlen(path_left) + 1 + strlen(path_right) + 1;
        format_string = "%s" STR_DIRSEP "%s";
    }
    path_buffer = (char*)malloc(cb_alloc);
    snprintf(path_buffer, cb_alloc, format_string, path_left, path_right);
    return path_buffer;
}

int BL_futils_size(const char* path, uint32_t* cb_filesize)
{
    int err = ESUCCESS;
    do {
        struct stat s;
        if (-1 == stat(path, &s))
        {
            err = errno;
            break;
        }
        *cb_filesize = (uint32_t)s.st_size;
    } while (0);
    return err;
}

int BL_futils_open(FILE* *ppf, const char* path, const char* mode)
{
    int err = ESUCCESS;
    do {
        if (*mode == 'a' || *mode == 'A' || *mode == 'w' || *mode == 'W')
        {
            char* dirpath = BL_futils_copydirpath(path);
            if (ESUCCESS != (err = BL_futils_create_directories(dirpath)))
            {
                break;
            }
            *ppf = fopen(path, mode);
            if (*ppf == NULL)
            {
                err = errno;
            }
        }
        else if (*mode == 'r' || *mode == 'R')
        {
            *ppf = fopen(path, mode);
            if (*ppf == NULL)
            {
                err = errno;
            }
        }
        else
        {
            err = EINVAL;
            break;
        }
    } while (0);
    return err;
}

#include "base_l/BL_putils.h"
#ifdef __linux__
#define MKDIR_CMD   "/usr/bin/mkdir"
#define RM_CMD      "/usr/bin/rm"
#else
#define MKDIR_CMD   "/bin/mkdir"
#define RM_CMD      "/bin/rm"
#endif

int BL_futils_create_directories(const char* path)
{
    if (strlen(path) == 0) return ESUCCESS;
    char cmd_path[] = MKDIR_CMD;
    char cmd_flag[] = "-p";
    char* const argv[] = {
        cmd_path, cmd_flag, strdup(path), NULL
    };
    int exit_code;
    int err = BL_putils_execv_sync(argv, &exit_code);
    free((void*)argv[2]);
    return err;
}


int BL_futils_remove_directories(const char* path)
{
    char cmd_path[] = RM_CMD;
    char cmd_flag[] = "-rf";
    char* const argv[] = {
        cmd_path, cmd_flag, strdup(path), NULL
    };
    int exit_code;
    int err = BL_putils_execv_sync(argv, &exit_code);
    free((void*)argv[2]);
    return err;
}

bool BL_futils_exists(const char* path)
{
    struct stat s;
    return (0 == stat(path, &s));
}

