#ifndef BL_SERIALIZEBASE_H_
#define BL_SERIALIZEBASE_H_
#include "base_l/BL_base.h"
#include "base_l/BL_types.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef const char* (*BL_formatter_t)(BL_cptr_t value_to_format);

typedef int (*BL_parser_t)(const char* src_string, BL_ptr_t parsed);

const char* BL_format_1i16(BL_cptr_t value);
const char* BL_format_1u16(BL_cptr_t value);
const char* BL_format_1i32(BL_cptr_t value);
const char* BL_format_1u32(BL_cptr_t value);

const char* BL_format_2i16(BL_cptr_t value);
const char* BL_format_2u16(BL_cptr_t value);
const char* BL_format_2i32(BL_cptr_t value);
const char* BL_format_2u32(BL_cptr_t value);

const char* BL_format_3i16(BL_cptr_t value);
const char* BL_format_3u16(BL_cptr_t value);
const char* BL_format_3i32(BL_cptr_t value);
const char* BL_format_3u32(BL_cptr_t value);

const char* BL_format_4i16(BL_cptr_t value);
const char* BL_format_4u16(BL_cptr_t value);
const char* BL_format_4i32(BL_cptr_t value);
const char* BL_format_4u32(BL_cptr_t value);

int BL_parse_1i16(const char* src_string, BL_ptr_t parsed);
int BL_parse_1u16(const char* src_string, BL_ptr_t parsed);
int BL_parse_1i32(const char* src_string, BL_ptr_t parsed);
int BL_parse_1u32(const char* src_string, BL_ptr_t parsed);

int BL_parse_2i16(const char* src_string, BL_ptr_t parsed);
int BL_parse_2u16(const char* src_string, BL_ptr_t parsed);
int BL_parse_2i32(const char* src_string, BL_ptr_t parsed);
int BL_parse_2u32(const char* src_string, BL_ptr_t parsed);

int BL_parse_3i16(const char* src_string, BL_ptr_t parsed);
int BL_parse_3u16(const char* src_string, BL_ptr_t parsed);
int BL_parse_3i32(const char* src_string, BL_ptr_t parsed);
int BL_parse_3u32(const char* src_string, BL_ptr_t parsed);

int BL_parse_4i16(const char* src_string, BL_ptr_t parsed);
int BL_parse_4u16(const char* src_string, BL_ptr_t parsed);
int BL_parse_4i32(const char* src_string, BL_ptr_t parsed);
int BL_parse_4u32(const char* src_string, BL_ptr_t parsed);
#ifdef __cplusplus
}
#endif
#endif /* BL_SERIALIZEBASE_H_ */