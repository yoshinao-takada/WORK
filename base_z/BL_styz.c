#include    "base_z/BL_styz.h"
#include    <base_l/BL_errno.h>

static BL_matfnc128RM_t matfnc = NULL;

pBL_styz_t BL_styz_new2(uint32_t port_count, const double* Zc)
{
    if (!matfnc)
    {
        matfnc = BL_matfnc128RM();
    }
    pBL_styz_t p = NULL;
    int err = ESUCCESS;
    do {
        // setup pointers
        uint32_t cb_header = sizeof(BL_styz_t);
        uint32_t ce_mat = (port_count * port_count); // element count per a matrix
        uint32_t cb_mat = ce_mat * sizeof(BL_1c128_t); // byte count per a matrix
        uint32_t cb_data = cb_mat * (BL_STYZ_WORKSIZE + 3); // total byte count for matrix elements
        uint32_t cb_alloc = cb_header + cb_data; // total alloc bytes
        p = (pBL_styz_t)malloc(cb_alloc);
        if (!p)
        {
            err = ENOMEM;
            break;
        }
        p->port_count = port_count;
        p->Sz = buf;
        p->Sy = p->Sz + ce_mat;
        p->U = p->Sy + ce_mat;
        for (uint32_t i = 0; i != BL_STYZ_WORKSIZE; i++)
        {
            p->work[i] = (i + 1) * ce_mat;
        }

        // fill U
        matfnc->unit(_1, p->U, port_count);

        // fill Sz
        matfnc->fill(_0, p->Sz, port_count, port_count);
        BL_1c128_t* pele = p->Sz;
        const double* pZc = Zc;
        for (uint32 i = 0; i != port_count; i++)
        {
            *pele = CMPLX(sqrt(*pZc++), 0.0);
            pele += (port_count + 1);
        }

        // fill Sy
        matfnc->fill(_0, p->Sy, port_count, port_count);
        pele = p->Sy;
        pZc = Zc;
        for (uint32 i = 0; i != port_count; i++)
        {
            *pele = CMPLX(1.0/sqrt(*pZc++), 0.0);
            pele += (port_count + 1);
        }
    } while (0);
    if (p && err)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

pBL_styz_t BL_styz_new1(uint32_t port_count, double Zc)
{
    pBL_styz_t p = NULL;
    double* pzc = BL_CALLOC(port_count, double);
    if (!pzc)
    {
        return p;
    }
    p = BL_styz_new2(port_count, pzc);
    BL_SAFEFREE(&psz);
    return p;
}

pBL_styz_t BL_styz_new(uint32_t port_count)
{
    return BL_styz_new1(port_count, 50.0);
}

