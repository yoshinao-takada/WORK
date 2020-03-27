#include "base_cg/BL_tristrip.h"
#include <math.h>

int BL_tristrip_strip(pcBL_oomesh_t mesh, BL_2u32_t at, pBL_linkable_t strip_anchor)
{
    const float nan = sqrtf(-1.0f);
    uint32_t offset = at[1] * mesh->wh[0];
    const BL_3r32_t* pvert = mesh->vert + offset;
    const BL_2r32_t* ptex = mesh->tex + offset;
    const uint8_t* pflags = mesh->flags + offset;
    bool retrieving = false;
    uint32_t count_retrieved = 0;
    int end_status = 0;
    uint32_t col = at[0];
    for (; col < mesh->wh[0]; col++)
    {
        // detect contiguous enabled vertices
        if (!retrieving && (*pflags & BL_oomesh_attr_enabled))
        {
            retrieving = true;
        }
        else if (retrieving && ((*pflags & BL_oomesh_attr_enabled) == 0))
        {
            break;
        }

        // if the vertex is enabled, copy data from the vertex to the new node
        if (retrieving)
        {
            count_retrieved++;
            pBL_linkable_array_t newnode = BL_linkable_array_new(6, float);
            BL_ptr_t node_data = BL_linkable_array_begin(newnode);
            node_data._1r32[0] = (*pvert)[0];
            node_data._1r32[1] = (*pvert)[1];
            node_data._1r32[2] = (*pvert)[2];
            node_data._1r32[3] = (*ptex)[0];
            node_data._1r32[4] = (*ptex)[1];
            node_data._1r32[5] = nan;
            BL_linkable_link_prev(strip_anchor, &(newnode->link));
        }

        // go to the next vertex
        pvert++;
        ptex++;
        pflags++;
    }

    // end-proessing
    at[0] = col;
    if (col != mesh->wh[0])
    {
        end_status |= BL_tristrip_strip_notendofrow;
    }
    if (count_retrieved < 3)
    {
        BL_linkable_unlink_other_than_anchor(strip_anchor);
    }
    else
    {
        end_status |= BL_tristrip_strip_withtriangles;
    }    
    return end_status;
}

pBL_tristrip_t BL_tristrip_new()
{
    pBL_tristrip_t p = (pBL_tristrip_t)malloc(sizeof(BL_tristrip_t));
    p->strip_anchor.prev = p->strip_anchor.next = &(p->strip_anchor);
    p->trunk.prev = p->trunk.next = &(p->trunk);
    return p;
}

pBL_tristrip_t BL_tristrip_clone(pcBL_tristrip_t instance)
{
    pBL_tristrip_t p = (pBL_tristrip_t)malloc(sizeof(BL_tristrip_t));
    p->strip_anchor.prev = instance->strip_anchor.prev;
    p->strip_anchor.next = instance->strip_anchor.next;
    p->trunk.prev = instance->trunk.prev;
    p->trunk.next = instance->trunk.next;
}

void BL_tristriplist_strip(pcBL_oomesh_t mesh, pBL_linkable_t trunk_anchor)
{
    for (uint32_t row = 0; row < mesh->wh[1]; row++)
    {
        BL_2u32_t at = { 0, row };
        int status = 0;
        do {
            BL_linkable_t anchor = { &anchor, &anchor };
            status = BL_tristrip_strip(mesh, at, &anchor);
            if (status & BL_tristrip_strip_withtriangles)
            {
                pBL_tristrip_t newnode = BL_tristrip_new();
                newnode->strip_anchor.next = anchor.next;
                newnode->strip_anchor.prev = anchor.prev;
                BL_linkable_link_prev(trunk_anchor, &(newnode->trunk));
            }
        } while (status & BL_tristrip_strip_notendofrow);
    }
}

