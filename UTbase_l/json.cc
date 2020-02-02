#include <gtest/gtest.h>
#include <base_l/BL_json.h>
#include <stdio.h>
#include <base_l/BL_base.h>
#include <base_l/BL_containers.h>
#include <base_l/BL_fsm.h>

#define SUIT    json
namespace 
{
    void dump_jsmn_token(const jsmntok_t* tok)
    {
        const char* label = JSMN_token_label(tok->type);
        printf("%s,%d,%d,%d\n", label, tok->start, tok->end, tok->size);
    }

    
    TEST(SUIT, parse_simple)
    {
        jsmntok_t tokens[16];
        const char* json_string = "{"
            "\"string_item1\" : \"value1\",\n"
            "\"array_item1\" : [1,2,3,4]\n"
        "}";

        enum {
            st_top,
            st_1st_layer,
            st_string_item1_value,
            st_array_item1_value,
            st_array_elements,
            st_end
        } state = st_top;


        struct {
            char* string_item1;
            pBL_array_t intarray;
        } json_struct = { NULL, NULL };

        printf("%s\n", json_string);

        jsmn_parser parser;
        jsmn_init(&parser);
        int r = jsmn_parse(&parser, json_string, strlen(json_string), tokens, ARRAYSIZE(tokens));
        printf("parse result = %d\n", r);
        if (r > 0)
        {
            for (int i = 0; i < r; i++)
            {
                dump_jsmn_token(tokens + i);
            }
        }
        {
            const jsmntok_t* ptok = tokens;
            uint32_t array_size = 0;
            uint32_t array_count = 0;
            uint32_t top_objects = 0;
            for (int i = 0; (i < r) & (state != st_end); i++, ptok++)
            {
                switch (state)
                {
                    case st_top:
                    {
                        top_objects = ptok->size;
                        state = st_1st_layer;
                        break;
                    }
                    case st_1st_layer:
                    {
                        ASSERT_EQ(JSMN_STRING, ptok->type);
                        if (JSMN_eq(json_string, ptok, "string_item1"))
                        {
                            state = st_string_item1_value;
                        }
                        else if (JSMN_eq(json_string, ptok, "array_item1"))
                        {
                            state = st_array_item1_value;
                        }
                        else
                        {
                            state = st_end;
                        }
                        break;
                    }
                    case st_string_item1_value:
                    {
                        json_struct.string_item1 = strndup(json_string + ptok->start, ptok->end - ptok->start);
                        printf("string_item1: %s\n", json_struct.string_item1);
                        state = st_1st_layer;
                        break;
                    }
                    case st_array_item1_value:
                    {
                        array_size = ptok->size;
                        json_struct.intarray = BL_array_new(array_size, BL_1i32_t);
                        state = st_array_elements;
                        break;
                    }
                    case st_array_elements:
                    {
                        const char* numstring = strndup(json_string + ptok->start, ptok->end - ptok->start);
                        BL_ptr_t array_begin = BL_array_begin(json_struct.intarray);
                        *(array_begin._1i32 + array_count++) = atoi(numstring);
                        if (array_count == array_size)
                        {
                            state = st_1st_layer;
                        }
                        free((void*)numstring);
                        break;
                    }
                    case st_end:
                    {
                        break;
                    }
                }
            }
            ASSERT_EQ(0, strcmp("value1", json_struct.string_item1));
            ASSERT_EQ(4, (int)(json_struct.intarray->unit_count));
            BL_cptr_t ptr = BL_array_cbegin(json_struct.intarray);
            ASSERT_EQ(1, *(ptr._1i32));
            ASSERT_EQ(2, *(ptr._1i32 + 1));
            ASSERT_EQ(3, *(ptr._1i32 + 2));
            ASSERT_EQ(4, *(ptr._1i32 + 3));
        }
        if (json_struct.string_item1)
        {
            free(json_struct.string_item1);
        }
        if (json_struct.intarray)
        {
            free(json_struct.intarray);
        }
    }


    typedef struct {
        char *string_item1, *string_item2;
        pBL_array_t array_item1;
        pBL_array_t array_item2;
    } json_struct_t, *pjson_struct_t;

    template<typename T> void clean_ptr(T** ppT)
    {
        if (ppT && (*ppT))
        {
            free((void*)(*ppT));
            *ppT = nullptr;
        }
    }
    void clean_json_struct(pjson_struct_t s)
    {
        clean_ptr(&(s->string_item1));
        clean_ptr(&(s->string_item2));
        clean_ptr(&(s->array_item1));
        clean_ptr(&(s->array_item2));
    }

    static BL_fsm_result_t st_top_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_1st_layer_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_array_item1_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_array_item2_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_string_item1_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_string_item2_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_array_item1_elements_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_array_item2_elements_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    static BL_fsm_result_t st_end_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg);

    typedef struct {
        const jsmntok_t *tok;
        const char* json_string;
        json_struct_t json_struct;
        uint32_t array_index;
    } json_parser_context_t, *pjson_parser_context_t;

    typedef enum {
        st_top,
        st_1st_layer,
        st_array_item1,
        st_array_item2,
        st_array_item1_elements,
        st_array_item2_elements,
        st_string_item1,
        st_string_item2,
        st_end
    } st_json_parser_t;

    const BL_fsm_state_t states[] =
    {
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_top_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_1st_layer_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_array_item1_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_array_item2_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_array_item1_elements_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_array_item2_elements_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_string_item1_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_string_item2_handler },
        { BL_fsm_ah_do_nothing, BL_fsm_ah_do_nothing, st_end_handler },
    };

    TEST(SUIT, parse_fsm)
    {
        const char* json_string = "{"
            "\"array_item1\" : [ 100, 101, 102 ],\n"
            "\"array_item2\" : [ 1, 10, 100, 1000 ],\n"
            "\"string_item1\" : \"string_value1\",\n"
            "\"string_item2\" : \"string_value2\"\n"
        "}";
        printf("%s\n", json_string);
        jsmntok_t tok[32];
        jsmn_parser parser;
        jsmn_init(&parser);
        int r = jsmn_parse(&parser, json_string, strlen(json_string), tok, ARRAYSIZE(tok));
        ASSERT_GT(r, 0);
        for (int i = 0; i < r; i++)
        {
            dump_jsmn_token(tok + i);
        }
        json_parser_context_t context = { tok, json_string, {nullptr,nullptr, nullptr, nullptr}, 0 };
        const jsmntok_t* tok_end = tok + r;
        st_json_parser_t state = st_top;

        pcBL_fsm_state_t state_end = &states[st_end];
        BL_fsm_t fsm =
        {
            &states[st_top],
            nullptr,
            (void*)&context
        };

        BL_fsm_msg_t msg;
        for (; (context.tok != tok_end) && (fsm.state != state_end); context.tok++)
        {
            msg.msgid = context.tok->type;
            msg.payload = (void*)context.tok;
            BL_fsm_eval(&fsm, &msg);
        }

        ASSERT_EQ(0, strcmp(context.json_struct.string_item1, "string_value1"));
        ASSERT_EQ(0, strcmp(context.json_struct.string_item2, "string_value2"));
        ASSERT_EQ(3, context.json_struct.array_item1->unit_count);
        ASSERT_EQ(4, context.json_struct.array_item2->unit_count);
        BL_cptr_t array1_ptr = BL_array_cbegin(context.json_struct.array_item1);
        ASSERT_EQ(100, *array1_ptr._1i32++);
        ASSERT_EQ(101, *array1_ptr._1i32++);
        ASSERT_EQ(102, *array1_ptr._1i32);
        BL_cptr_t array2_ptr = BL_array_cbegin(context.json_struct.array_item2);
        ASSERT_EQ(1, *array2_ptr._1i32++);
        ASSERT_EQ(10, *array2_ptr._1i32++);
        ASSERT_EQ(100, *array2_ptr._1i32++);
        ASSERT_EQ(1000, *array2_ptr._1i32++);
        clean_json_struct(&context.json_struct);
    }


    BL_fsm_result_t st_top_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        do {
            fsm->state = &states[st_1st_layer];
        } while (false);
        return result;
    }

    BL_fsm_result_t st_1st_layer_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t *tok = (const jsmntok_t*)(msg->payload);
        do {
            if (JSMN_eq(context.json_string, tok, "array_item1"))
            {
                fsm->state = &states[st_array_item1];
            }
            else if (JSMN_eq(context.json_string, tok, "array_item2"))
            {
                fsm->state = &states[st_array_item2];
            }
            else if (JSMN_eq(context.json_string, tok, "string_item1"))
            {
                fsm->state = &states[st_string_item1];
            }
            else if (JSMN_eq(context.json_string, tok, "string_item2"))
            {
                fsm->state = &states[st_string_item2];
            }
            else
            {
                fsm->state = &states[st_end];
            }
        } while (false);
        return result;
    }

    BL_fsm_result_t st_array_item1_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);
        do {
            if (msg->msgid == JSMN_ARRAY)
            {
                fsm->state = &states[st_array_item1_elements];
                context.json_struct.array_item1 = BL_array_new(tok->size, BL_1i32_t);
                context.array_index = 0;
            }
            else
            {
                fsm->state = &states[st_end];
            }
        } while (false);
        return result;
    }

    BL_fsm_result_t st_array_item2_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);
        do {
            if (msg->msgid == JSMN_ARRAY)
            {
                fsm->state = &states[st_array_item2_elements];
                context.json_struct.array_item2 = BL_array_new(tok->size, BL_1i32_t);
                context.array_index = 0;
            }
            else
            {
                fsm->state = &states[st_end];
            }
        } while (false);
        return result;
    }

    BL_fsm_result_t st_string_item1_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);
        do {
            fsm->state = &states[st_1st_layer];
            context.json_struct.string_item1 = strndup(context.json_string + tok->start, tok->end - tok->start);
        } while (false);
        return result;
    }

    BL_fsm_result_t st_string_item2_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);
        do {
            fsm->state = &states[st_1st_layer];
            context.json_struct.string_item2 = strndup(context.json_string + tok->start, tok->end - tok->start);
        } while (false);
        return result;
    }

    BL_fsm_result_t st_array_item1_elements_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_no_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);        
        char* buf = nullptr;
        do {
            buf = strndup(context.json_string + tok->start, tok->end - tok->start);
            BL_ptr_t array_begin= BL_array_begin(context.json_struct.array_item1);
            *(array_begin._1i32 + context.array_index) = atoi(buf);
            context.array_index++;
            if (context.array_index == context.json_struct.array_item1->unit_count)
            {
                result = BL_fsm_result_transit;
                fsm->state = &states[st_1st_layer];
            }
        } while (false);
        clean_ptr(&buf);
        return result;
    }

    BL_fsm_result_t st_array_item2_elements_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_no_transit;
        json_parser_context_t& context = *(pjson_parser_context_t)(fsm->context);
        const jsmntok_t* tok = (const jsmntok_t*)(msg->payload);        
        char* buf = nullptr;
        do {
            buf = strndup(context.json_string + tok->start, tok->end - tok->start);
            BL_ptr_t array_begin= BL_array_begin(context.json_struct.array_item2);
            *(array_begin._1i32 + context.array_index) = atoi(buf);
            context.array_index++;
            if (context.array_index == context.json_struct.array_item2->unit_count)
            {
                result = BL_fsm_result_transit;
                fsm->state = &states[st_1st_layer];
            }
        } while (false);
        clean_ptr(&buf);
        return result;
    }

    BL_fsm_result_t st_end_handler(pBL_fsm_t fsm, pBL_fsm_msg_t msg)
    {
        BL_fsm_result_t result = BL_fsm_result_no_transit;
        do {

        } while (false);
        return result;
    }
}