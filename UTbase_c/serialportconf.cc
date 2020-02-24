#include <gtest/gtest.h>
#include <base_c/BL_serialportconf.h>
#include <stdio.h>

#define SUIT    serialportconf
namespace{
    TEST(SUIT, tojson)
    {
        pBL_serialportconf_and_name_t pconf = BL_serialportconf_and_name_new2(BL_SERIALNAME_DEFAULT0);
        const char* pjson = BL_serialportconf_tojson(pconf);
        printf("%s\n", pjson);
        pBL_serialportconf_and_name_t pconf2 = nullptr;
        int err = BL_serialportconf_fromjson(pjson, &pconf2);
        ASSERT_EQ(ESUCCESS, err);
        ASSERT_NE(nullptr, pconf2);
        ASSERT_EQ(pconf->base.baudrate, pconf2->base.baudrate);
        free((void*)pjson);
        free((void*)pconf);
        free((void*)pconf2);
    }
}