#ifndef BL_FUTILS_H_
#define BL_FUTILS_H_
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif
    void BL_futils_split(char* pathstr);

    char* BL_futils_append(const char* path_left, const char* path_right);

    uint32_t BL_futils_size(const char* path);

    int BL_futils_open(FILE* *ppf, const char* path, const char* mode);

    int BL_futils_create_directories(const char* path);

    int BL_futils_remove_directories(const char* path);
#ifdef __cplusplus
}
#endif
#endif