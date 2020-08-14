#include    "base_l/BL_containers.h"

pBL_array_t BL_alloc_array(uint32_t unit_count, uint32_t unit_size)
{
    pBL_array_t p = (pBL_array_t)calloc(BL_CB_ARRAY_HEAD + unit_count * unit_size, 1);
    if (p == NULL)
    {
        return p;
    }
    p->unit_count = unit_count;
    return p;
}

pBL_linkable_array_t BL_alloc_linkable_array(uint32_t unit_count, uint32_t unit_size)
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

pBL_linkable_t BL_linkable_unlink_prev(pBL_linkable_t anchor)
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

pBL_linkable_t BL_linkable_unlink_next(pBL_linkable_t anchor)
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

pcBL_linkable_t BL_linkable_find(pcBL_linkable_t anchor, const void* conditions, BL_LINKABLE_MATCH test_func)
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

uint32_t BL_linkable_count(pcBL_linkable_t anchor)
{
    uint32_t c = 0;
    for (pcBL_linkable_t scanner = anchor->next; scanner != anchor; scanner = scanner->next)
    {
        c++;
    }
    return c;
}

int BL_linkable_delete(pBL_linkable_t anchor, const void* conditions, BL_LINKABLE_MATCH test_func)
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

// delete all nodes other than anchor
void BL_linkable_unlink_other_than_anchor(pBL_linkable_t anchor)
{
    pBL_linkable_t removed = NULL;
    while (NULL != (removed = BL_linkable_unlink_next(anchor)))
    {
        free((void*)removed);
    }
}

// delete all nodes including anchor
void BL_linkable_delete_all(pBL_linkable_t anchor)
{
    BL_linkable_unlink_other_than_anchor(anchor);
    free((void*)anchor);
}
uint32_t BL_product_dims(const BL_arrayMD_dims dims)
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

void BL_copy_dims(const BL_arrayMD_dims dims_src, BL_arrayMD_dims dims_dst)
{
    const uint64_t* ptr_src = (const uint64_t*)dims_src;
    uint64_t* ptr_dst = (uint64_t*)dims_dst;
    *ptr_dst++ = *ptr_src++;
    *ptr_dst = *ptr_src;
}

pBL_arrayMD_t BL_alloc_arrayMD(const BL_arrayMD_dims dims, uint32_t unit_size)
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
