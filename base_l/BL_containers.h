#ifndef BL_CONTAINERS_H_
#define BL_CONTAINERS_H_
#include "base_l/BL_base.h"
#include "base_l/BL_types.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    uintptr_t unit_count;
} BL_array_t, *pBL_array_t;

typedef const BL_array_t *pcBL_array_t;

#define BL_free(ppobj)  if (ppobj && *ppobj) { free(*ppobj); *ppobj = NULL; }

#define BL_CB_ARRAY_HEAD    sizeof(BL_array_t)

inline pBL_array_t BL_alloc_aray(uint32_t unit_count, uint32_t unit_size)
{
    pBL_array_t p = (pBL_array_t)calloc(BL_CB_ARRAY_HEAD + unit_count * unit_size, 1);
    if (p == NULL)
    {
        return p;
    }
    p->unit_count = unit_count;
    return p;
}

#define BL_array_new(unit_count_, unit_type_)  BL_alloc_aray(unit_count_, sizeof(unit_type_))
#define BL_array_begin(ptr)   { (int8_t*)((ptr) + 1) }
#define BL_array_cbegin(ptr)  { (const int8_t*)((ptr) + 1) }
#define BL_array_end(ptr, unit_type_) \
    { (int8_t*)((ptr) + 1) + ((ptr)->unit_count * sizeof(unit_type_)) }
#define BL_array_cend(ptr, unit_type_)  \
    { (const int8_t*)((ptr) + 1) + ((ptr)->unit_count * sizeof(unit_type_)) }

typedef struct BL_linkable {
    struct BL_linkable *prev;
    struct BL_linkable *next;
} BL_linkable_t, *pBL_linkable_t;

typedef const BL_linkable_t* pcBL_linkable_t;

#define BL_linkable_declare_anchor(name)    BL_linkable_t name = { &name, &name }
typedef struct {
    BL_linkable_t link;
    BL_array_t data;
} BL_linkable_array_t, *pBL_linkable_array_t;

typedef const BL_linkable_array_t *pcBL_linkable_array_t;

#define BL_CB_LINKABLE_ARRAY_HEAD   sizeof(BL_linkable_array_t)
#define BL_CB_LINKABLE_ARRAY(unit_count_, unit_type_) \
   (BL_CB_LINKABLE_ARRAY_HEAD + BL_CB_ARRAY_DATA(unit_count_, unit_type_))

inline pBL_linkable_array_t BL_alloc_linkable_array(uint32_t unit_count, uint32_t unit_size)
{
    pBL_linkable_array_t p = (pBL_linkable_array_t)calloc(BL_CB_LINKABLE_ARRAY_HEAD + unit_count * unit_size, 1);
    if (p == NULL)
    {
        return p;
    }
    p->link.next = p->link.prev = &(p->link);
    p->data.unit_count = unit_count;
    return p;
}

#define BL_linkable_array_new(unit_count_, unit_type_)  BL_alloc_linkable_array(unit_count_, sizeof(unit_type_))
#define BL_linkable_array_begin(ptr)    BL_array_begin(ptr)
#define BL_linkable_array_cbegin(ptr)    BL_array_cbegin(ptr)
#define BL_linkable_array_end(ptr)      BL_array_end(ptr)
#define BL_linkable_array_cend(ptr)      BL_array_cend(ptr)

#define BL_linkable_link_prev(anchor, node) { \
    (anchor)->prev->next = node; \
    (node)->prev = (anchor)->prev; \
    (node)->next = (anchor); \
    (anchor)->prev = node; \
}

#define BL_linkable_link_next(anchor, node) { \
    (anchor)->next->prev = (node); \
    (node)->next = (anchor)->next; \
    (node)->prev = (anchor); \
    (anchor)->next = (node); \
}

inline pBL_linkable_t BL_linkable_remove_prev(pBL_linkable_t anchor)
{
    pBL_linkable_t removed = NULL;
    if (anchor->prev != anchor)
    {
        removed = anchor->prev;
        removed->prev->next = anchor;
        anchor->prev = removed->prev;
        removed->prev = removed->next = removed;
    }
    return removed;
}

inline pBL_linkable_t BL_linkable_remove_next(pBL_linkable_t anchor)
{
    pBL_linkable_t removed = NULL;
    if (anchor->prev != anchor)
    {
        removed = anchor->next;
        removed->next->prev = anchor;
        anchor->next = removed->next;
        removed->prev = removed->next = removed;
    }
    return removed;
}

typedef bool (*BL_LINKABLE_MATCH)(pcBL_linkable_t node, const void* conditions);

inline pcBL_linkable_t BL_linkable_find(pcBL_linkable_t anchor, const void* conditions, BL_LINKABLE_MATCH test_func)
{
    pcBL_linkable_t found = NULL;
    for (pBL_linkable_t scanner = anchor->next; scanner != anchor; scanner = scanner->next)
    {
        if (test_func(scanner, conditions))
        {
            found = scanner;
            break;
        }
    }
    return found;
}

inline uint32_t BL_linkable_count(pcBL_linkable_t anchor)
{
    uint32_t c = 0;
    for (pcBL_linkable_t scanner = anchor->next; scanner != anchor; scanner = scanner->next)
    {
        c++;
    }
    return c;
}

typedef uint16_t    BL_arrayMD_dims[8];

typedef struct {
    BL_arrayMD_dims dims;
    BL_array_t data;
} BL_arrayMD_t, *pBL_arrayMD_t;

typedef const BL_arrayMD_t *pcBL_arrayMD_t;

#define BL_CB_ARRAYMD_HEAD   sizeof(BL_arrayMD_t)
inline uint32_t BL_product_dims(const BL_arrayMD_dims dims)
{
    uint32_t prod = 1;
    for (uint16_t i = 0; i < 8; i++)
    {
        if (dims[i])
        {
            prod *= dims[i];
        }
        else
        {
            break;
        }
    }
    return prod;
}

inline void BL_copy_dims(const BL_arrayMD_dims dims_src, BL_arrayMD_dims dims_dst)
{
    const uint64_t* ptr_src = (const uint64_t*)dims_src;
    uint64_t* ptr_dst = (uint64_t*)dims_dst;
    *ptr_dst++ = *ptr_src++;
    *ptr_dst = *ptr_src;
}

inline pBL_arrayMD_t BL_alloc_arrayMD(const BL_arrayMD_dims dims, uint32_t unit_size)
{
    uint32_t unit_count = BL_product_dims(dims);
    pBL_arrayMD_t p = (pBL_arrayMD_t)calloc(BL_CB_ARRAYMD_HEAD + unit_size * unit_count, 1);
    if (p == NULL)
    {
        return p;
    }
    BL_copy_dims(dims, p->dims);
    p->data.unit_count = unit_count;
    return p;
}
#define BL_arrayMD_new(dims_, unit_type_)  BL_alloc_arrayMD(dims_, sizeof(unit_type_))
#define BL_arrayMD_begin(ptr)   BL_array_begin(ptr)
#define BL_arrayMD_cbegin(ptr)  BL_array_cbegin(ptr)
#define BL_arrayMD_end(ptr, unit_type_) BL_array_end(ptr, unit_type_)
#define BL_arrayMD_cend(ptr, unit_type_)    BL_array_cend(ptr, unit_type_)

#ifdef __cplusplus
}
#endif
#endif