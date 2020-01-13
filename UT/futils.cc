#include "gtest/gtest.h"
#include "base_l/BL_base.h"
#include "base_l/BL_futils.h"

#define SUIT    futils

namespace
{
    TEST(SUIT, split_path)
    {
        static const char* ref_str[] = { "123", "456", "abc", "def" };
        char path_str[] = "/123/456/abc/def";
        const char* end_path_str = path_str + strlen(path_str);
        BL_futils_split(path_str);
        char* scanner = path_str;
        for (int i = 0; i < ARRAYSIZE(ref_str); i++)
        {
            while (*scanner == '\0') scanner++;
            ASSERT_EQ(0, strcmp(ref_str[i], scanner));
            scanner += strlen(scanner) + 1;
        }
    }

    TEST(SUIT, findfilename)
    {
        static const char* filepaths[] = {
            "/usr/local/bin/sh",
            "only filename.pdf",
            "/a file placed at root"
        };

        const char ref_char[] = { 's', 'o', 'a' };

        static const char* dirpaths[] = {
            "/usr/local/bin/",
            "",
            "/"
        };

        for (int i = 0; i < ARRAYSIZE(filepaths); i++)
        {
            const char* fname_begin = BL_futils_findfilename(filepaths[i]);
            ASSERT_EQ(ref_char[i], *fname_begin);
        }

        for (int i = 0; i < ARRAYSIZE(filepaths); i++)
        {
            char* dirpath = BL_futils_copydirpath(filepaths[i]);
            ASSERT_EQ(0, strcmp(dirpaths[i], dirpath));
            free((void*)dirpath);
        }
    }

    TEST(SUIT, append)
    {
        const char* left_parts[] = { "/usr/bin", "/usr/bin/" };
        const char* right_parts[] = { "bash", "bash"};
        const char* ref_str[] = { "/usr/bin/bash", "/usr/bin/bash" };
        for (int i = 0; i < ARRAYSIZE(left_parts); i++)
        {
            char* combined = BL_futils_append(left_parts[i], right_parts[i]);
            ASSERT_EQ(0, strcmp(ref_str[i], combined));
            free((void*)combined);
        }
    }

#define SAMPLE_FILE "/home/yoshinao/REPOS/WORK_A0/UTData/In/sample-abc.txt"
    TEST(SUIT, filesize)
    {
        uint32_t cb_filesize;
        int err = BL_futils_size(SAMPLE_FILE, &cb_filesize);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_EQ(4, cb_filesize);
    }

#define TEST_DIRECTORIES    "/home/yoshinao/REPOS/WORK_A0/UTData/Out/1/2/3"
#define TEST_DIRECTORIES2   "/home/yoshinao/REPOS/WORK_A0/UTData/Out/1"

    TEST(SUIT, create_remove_directories)
    {
        int err = BL_futils_create_directories(TEST_DIRECTORIES);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_EQ(true, BL_futils_exists(TEST_DIRECTORIES));

        err = BL_futils_remove_directories(TEST_DIRECTORIES2);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_EQ(false, BL_futils_exists(TEST_DIRECTORIES2));
    }
} // namespace

