#define JSMN_STATIC
#include "base_l/BL_json.h"
#include "base_l/BL_base.h"
#include <string.h>

const char* JSMN_token_label(jsmntype_t jt)
{
    static const char* token_labels[] =
    {
        "JSMN_UNDEFINED",
        "JSMN_OBJECT",
        "JSMN_ARRAY",
        "JSMN_STRING",
        "JSMN_PRIMITIVE",
    };
    return ((int)jt < ARRAYSIZE(token_labels)) ? token_labels[jt] : "INVALID";
}

bool JSMN_eq(const char* str_json, const jsmntok_t* ptok, const char* str_ref)
{
    size_t cmp_length = __min(strlen(str_ref), ptok->end - ptok->start);
    int cmp_result = strncmp(str_json + ptok->start, str_ref, cmp_length);
    return (0 == cmp_result);
}