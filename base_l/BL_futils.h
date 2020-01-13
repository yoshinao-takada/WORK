#ifndef BL_FUTILS_H_
#define BL_FUTILS_H_
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif
    /*!
    \brief replace directory delimiter character with '\0'
    */
    void BL_futils_split(char* pathstr);

    /*!
    \brief return the pointer to the beginning of the filename base.
    for example, filepath = "/hello-world/123.txt", the return pointer points to
    character '1'.
    filepath = "only filename.pdf", the return pointer points to character 'o'.
    */
    const char* BL_futils_findfilename(const char* filepath);

    /*!
    \brief copy the directory part of filepath.
    for example, filepath = "/usr/local/bin/sh", return pointer holds "/usr/local/bin/".
    Caller code must free() to release the returned directory path string.
    \param filepath [in] path string
    \return directory part of filepath.
    */
    char* BL_futils_copydirpath(const char* filepath);

    /*!
    \brief create a new path string combining two path strings.
    The left one can be terminated or unterminated with directory delimiter character.
    \param path_left [in] left part of the path string.
    \param path_right [in] right part of the path string.
    \return path_left + path_right.
    for example, path_left = "/usr/bin", path_right = "bash", return = "/usr/bin/bash",
    another example, path_left = "/usr/bin/", path_right = "bash", return = "/usr/bin/bash"
    */
    char* BL_futils_append(const char* path_left, const char* path_right);

    
    int BL_futils_size(const char* path, uint32_t* cb_filesize);

    int BL_futils_open(FILE* *ppf, const char* path, const char* mode);

    int BL_futils_create_directories(const char* path);

    int BL_futils_remove_directories(const char* path);

    bool BL_futils_exists(const char* path);
#ifdef __cplusplus
}
#endif
#endif