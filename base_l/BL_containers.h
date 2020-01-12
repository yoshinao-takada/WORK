#ifndef BL_CONTAINERS_H_
#define BL_CONTAINERS_H_
#include "base_l/BL_base.h"
#include "base_l/BL_types.h"
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    uintptr_t unit_count;
} BL_array_t, *pBL_array_t;

typedef const BL_array_t *pcBL_array_t;

#define BL_free(ppobj)  if (ppobj && *ppobj) { free(*ppobj); *ppobj = NULL; }

#define BL_CB_ARRAY_HEAD    sizeof(BL_array_t)

inline pBL_array_t BL_alloc_array(uint32_t unit_count, uint32_t unit_size)
{
    pBL_array_t p = (pBL_array_t)calloc(BL_CB_ARRAY_HEAD + unit_count * unit_size, 1);
    if (p == NULL)
    {
        return p;
    }
    p->unit_count = unit_count;
    return p;
}

#define BL_array_new(unit_count_, unit_type_)  BL_alloc_array(unit_count_, sizeof(unit_type_))
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

inline pBL_linkable_t BL_linkable_unlink_prev(pBL_linkable_t anchor)
{
    pBL_linkable_t unlinked = NULL;
    if (anchor->prev != anchor)
    {
        unlinked = anchor->prev;
        unlinked->prev->next = anchor;
        anchor->prev = unlinked->prev;
        unlinked->prev = unlinked->next = unlinked;
    }
    return unlinked;
}

inline pBL_linkable_t BL_linkable_unlink_next(pBL_linkable_t anchor)
{
    pBL_linkable_t unlinked = NULL;
    if (anchor->prev != anchor)
    {
        unlinked = anchor->next;
        unlinked->next->prev = anchor;
        anchor->next = unlinked->next;
        unlinked->prev = unlinked->next = unlinked;
    }
    return unlinked;
}

/*!
\brief Testing condition matching function type
\param node [in] node of linked list
\param conditions [in] anything casted to void ptr.
\return true: matched, false: NOT matched
*/
typedef bool (*BL_LINKABLE_MATCH)(pcBL_linkable_t node, const void* conditions);

/*!
\brief find a node satisfying the conditions given by conditions and test_func.
\param anchor [in] anchor node of the linked list to search. The anchor node is not a test subject.
Nodes other than anchor are test subjects. only the 1st one is returned even if multiple nodes
satisfies the matching condition.
\param conditions [in] test condition
\param test_func [in] test function comparing conditions and contents of the nodes in the list.
\return NULL: NOT found, any pointer: found node
*/
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

inline int BL_linkable_delete(pBL_linkable_t anchor, const void* conditions, BL_LINKABLE_MATCH test_func)
{
    pcBL_linkable_t found = BL_linkable_find(anchor, conditions, test_func);
    if (found == NULL)
    {
        return ENOLINK;
    }
    pBL_linkable_t prev = found->prev;
    pBL_linkable_t to_remove = BL_linkable_unlink_next(prev);
    free((void*)to_remove);
    return ESUCCESS;
}

inline void BL_linkable_unlink_other_than_anchor(pBL_linkable_t anchor)
{
    pBL_linkable_t removed = NULL;
    while (NULL != (removed = BL_linkable_unlink_next(anchor)))
    {
        free((void*)removed);
    }
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