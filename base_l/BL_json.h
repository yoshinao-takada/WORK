#ifndef BL_JSON_H_
#define BL_JSON_H_
#include "jsmn.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

    /*!
    \brief convert an enum value to a corresponding label string.
    */
    const char* JSMN_token_label(jsmntype_t jt);

    /*!
    \brief check equiality between the tokenized item and a reference string
    \param str_json [in] JSON source string
    \param ptok [in] a token created by jsmn parser
    \param str_ref [in] reference string
    \return true: token string and str_ref matched. false: otherwise
    */
    bool JSMN_eq(const char* str_json, const jsmntok_t* ptok, const char* str_ref);

#ifdef __cplusplus
}
#endif
#endif /* BL_JSON_H_ */