#include "gtest/gtest.h"
#include "base_l/BL_base.h"
#include "base_l/BL_containers.h"

#define SUIT    containers

namespace
{
    /*!
    \brief BL_array_new(), confirm begin(),end(), cbegin(), cend(), unit_count.
    */
    TEST(SUIT, array_new)
    {
        pBL_array_t parray0 = BL_array_new(7,uint16_t);
        ASSERT_EQ(7, parray0->unit_count);
        ASSERT_EQ((void*)(parray0 + 1), (void*)BL_array_begin(parray0));
        BL_ptr_t begin0 = BL_array_begin(parray0);
        BL_ptr_t end0 = BL_array_end(parray0, uint16_t);
        BL_cptr_t cbegin0 = BL_array_cbegin(parray0);
        BL_cptr_t cend0 = BL_array_cend(parray0, uint16_t);
        ASSERT_EQ(7, end0._1u16 - begin0._1u16);
        ASSERT_EQ((const void*)begin0._1u16, (const void*)cbegin0._1u16);
        ASSERT_EQ((const void*)end0._1u16, (const void*)cend0._1u16);
        BL_free(&parray0);
    }
    
    /*!
    \brief BL_linkable_array_t, linke_prev, remove_next, link_next, remove_prev.
    */
    TEST(SUIT, linkable_array_new_link_remove_free)
    {
        BL_linkable_declare_anchor(anchor);
        pBL_linkable_array_t nodes[4] = { NULL, NULL, NULL, NULL };
        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            nodes[i] = BL_linkable_array_new((i + 10) * 33, uint32_t);
            BL_linkable_link_prev(&anchor, &(nodes[i]->link));
        }
        uint32_t node_count = BL_linkable_count(&anchor);
        ASSERT_EQ(ARRAYSIZE(nodes), node_count);
        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            pBL_linkable_array_t node = (pBL_linkable_array_t)BL_linkable_remove_next(&anchor);
            ASSERT_EQ((void*)nodes[i], (void*)node);
        }

        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            BL_linkable_link_next(&anchor, &(nodes[i]->link));
        }
        node_count = BL_linkable_count(&anchor);
        ASSERT_EQ(ARRAYSIZE(nodes), node_count);
        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            pBL_linkable_array_t node = (pBL_linkable_array_t)BL_linkable_remove_prev(&anchor);
            ASSERT_EQ((void*)nodes[i], (void*)node);
            BL_free(&node);
        }
    }

    /*!
    \brief BL_linkable_array_t comparison callback function.
    \param node [in] linkable array to test. containing int32_t numbers
    \param ref_array_ [in] array as test parameters. containing int32_t numbers
    */
    static bool EqualArray(pcBL_linkable_t node_, const void* ref_array_)
    {
        pcBL_linkable_array_t node = (pcBL_linkable_array_t)node_;
        pcBL_array_t ref_array = (pcBL_array_t)ref_array_;
        bool b = node->data.unit_count == ref_array->unit_count;
        if (b)
        {
            BL_cptr_t node_begin = BL_linkable_array_cbegin(node);
            BL_cptr_t ref_begin = BL_array_cbegin(ref_array);

            for (int i=0; i< ref_array->unit_count; i++)
            {
                b &= node_begin._1i32[i] == ref_begin._1i32[i];
            }
        }
        return b;
    }

    /*!
    \brief BL_linkable_array_t find node.
    */    
    TEST(SUIT, linkable_array_find_node)
    {
        BL_linkable_declare_anchor(anchor);
        static const uint32_t node_sizes[] = { 101,102,103, 104 };
        pBL_linkable_array_t nodes[] = {
            BL_linkable_array_new(node_sizes[0], int32_t),
            BL_linkable_array_new(node_sizes[1], int32_t),
            BL_linkable_array_new(node_sizes[2], int32_t),
            BL_linkable_array_new(node_sizes[3], int32_t)
        };
        // only node[2] is filled with sequencial numbers.
        pBL_array_t ref_array = BL_array_new(node_sizes[2], int32_t);
        BL_ptr_t ref_begin = BL_array_begin(ref_array);
        BL_ptr_t node_2_begin = BL_linkable_array_begin(nodes[2]);
        for (int i = 0; i < nodes[2]->data.unit_count; i++)
        {
            ref_begin._1i32[i] = i;
            node_2_begin._1i32[i] = i;
        }
        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            BL_linkable_link_next(&anchor, &(nodes[i]->link));
        }

        pBL_linkable_array_t found = (pBL_linkable_array_t)BL_linkable_find(&anchor, (const void*)ref_array, EqualArray);
        ASSERT_EQ((const void*)found, (const void*)nodes[2]);
        for (int i = 0; i < ARRAYSIZE(nodes); i++)
        {
            BL_free(&nodes[i]);
        }
    }

    /*!
    \brief BL_arrayMD_t new, free
    */
    TEST(SUIT, arrayMD_new_free)
    {
        BL_arrayMD_dims dims = { 33, 37, 22 };
        pBL_arrayMD_t p = BL_arrayMD_new(dims, BL_3i32_t);
        ASSERT_EQ(33 * 37 * 22, p->data.unit_count);
        BL_free(&p);
    }
} // namespace

