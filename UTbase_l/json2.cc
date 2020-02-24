#include <rapidjson/document.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <base_l/BL_base.h>
#include <base_l/BL_containers.h>

#define SUIT    json2

typedef enum {
    nb_false,
    nb_true,
    nb_null
} nb_t; // null-able bool type
namespace 
{
    TEST(SUIT, simple0)
    {
        const char* json_string = "{"
            "\"Hello\": \"world\","
            "\"t\": true,"
            "\"f\": false,"
            "\"n\": null,"
            "\"i\": 123,"
            "\"pi\": 3.1416,"
            "\"a\": [1, 2, 3, 4]"
        "}";

        rapidjson::Document doc;
        doc.Parse(json_string);
        ASSERT_TRUE(doc.IsObject());
        ASSERT_TRUE(doc.HasMember("Hello"));
        ASSERT_FALSE(doc.HasMember("world"));
        ASSERT_TRUE(doc["a"].IsArray());
        rapidjson::Value& obja = doc["a"];
        ASSERT_EQ(4, obja.Size());
    }

    TEST(SUIT, construct0)
    {
        rapidjson::Document d;
        rapidjson::Value o(rapidjson::kObjectType);
    }
}