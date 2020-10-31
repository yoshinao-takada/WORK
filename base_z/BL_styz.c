#include    "base_z/BL_styz.h"
#include    <base_l/BL_errno.h>
#include    <malloc.h>

static pcBL_matfnc128RM_t matfnc = NULL;

pBL_styz_t BL_styz_new2(uint32_t port_count, const double* Zc)
{
    if (!matfnc)
    { // initialize matrix utility function table if it is used for the 1st time.
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
        p->Sz = p->buf;
        p->Sy = p->Sz + ce_mat;
        p->IMat = p->Sy + ce_mat;
        for (uint32_t i = 0; i != BL_STYZ_WORKSIZE; i++)
        {
            p->work[i] = p->IMat + (i + 1) * ce_mat;
        }

        // fill IMat
        matfnc->unit(CMPLX(1.0, 0.0), p->IMat, port_count);

        // fill Sz
        matfnc->fill(CMPLX(0.0,0.0), p->Sz, port_count, port_count);
        BL_1c128_t* pele = p->Sz;
        const double* pZc = Zc;
        for (uint32_t i = 0; i != port_count; i++)
        {
            *pele = CMPLX(sqrt(*pZc++), 0.0);
            pele += (port_count + 1);
        }

        // fill Sy
        matfnc->fill(CMPLX(0.0,0.0), p->Sy, port_count, port_count);
        pele = p->Sy;
        pZc = Zc;
        for (uint32_t i = 0; i != port_count; i++)
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
    for (uint32_t i = 0; i != port_count; i++)
    {
        pzc[i] = Zc;
    }
    p = BL_styz_new2(port_count, pzc);
    BL_SAFEFREE(&pzc);
    return p;
}

pBL_styz_t BL_styz_new(uint32_t port_count)
{
    return BL_styz_new1(port_count, 50.0);
}

const BL_1c128_t* BL_styz_zs(pBL_styz_t p, const BL_1c128_t* z, uint32_t index_offset)
{
    // work[0] = Sy * z, work[1] = work[0] * Sy, work[1] = Zn
    matfnc->mul(p->Sy, z, p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
    matfnc->mul(p->work[index_offset+0], p->Sy, p->work[index_offset+1], p->port_count, p->port_count, p->port_count);

    // work[0] = Zn + I
    matfnc->add(p->IMat, p->work[index_offset+1], p->work[index_offset+0], p->port_count, p->port_count);
    // work[2] = Zn - I
    matfnc->sub(p->work[index_offset+1], p->IMat, p->work[index_offset+2], p->port_count, p->port_count);
    // work[1] = inv(work[0])
    matfnc->inv(p->work[index_offset+0], p->work[index_offset+1], p->work[index_offset+3], p->port_count);
    // s = work[0] = work[1] * work[2]
    return matfnc->mul(p->work[index_offset+1], p->work[index_offset+2], p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
}

const BL_1c128_t* BL_styz_sz(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset)
{
    // work[0] = IMat + s, work[1] = IMat - s
    matfnc->add(p->IMat, s, p->work[index_offset+0], p->port_count, p->port_count);
    matfnc->sub(p->IMat, s, p->work[index_offset+1], p->port_count, p->port_count);
    // work[2] = inv(work[1])
    matfnc->inv(p->work[index_offset+1], p->work[index_offset+2], p->work[index_offset+3], p->port_count);
    // work[3] = work[0] * work[2]
    matfnc->mul(p->work[index_offset+0], p->work[index_offset+2], p->work[index_offset+3], p->port_count, p->port_count, p->port_count);
    // work[2] = Sz * work[3], work[0] = work[2] * Sz
    matfnc->mul(p->Sz, p->work[index_offset+3], p->work[index_offset+2], p->port_count, p->port_count, p->port_count);
    return matfnc->mul(p->work[index_offset+2], p->Sz, p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
}

const BL_1c128_t* BL_styz_ys(pBL_styz_t p, const BL_1c128_t* y, uint32_t index_offset)
{
    // work[0] = Sz * y, work[1] = work[0] * Sz = Sz * y * Sz = Yn
    matfnc->mul(p->Sz, y, p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
    matfnc->mul(p->work[index_offset+0], p->Sz, p->work[index_offset+1], p->port_count, p->port_count, p->port_count);
    // work[0] = U + Yn, work[2] = U - Yn
    matfnc->add(p->IMat, p->work[index_offset+1], p->work[index_offset+0], p->port_count, p->port_count);
    matfnc->sub(p->IMat, p->work[index_offset+1], p->work[index_offset+2], p->port_count, p->port_count);
    // work[1] = inv(work[0])
    matfnc->inv(p->work[index_offset+0], p->work[index_offset+1], p->work[index_offset+3], p->port_count);
    return matfnc->mul(p->work[index_offset+1], p->work[index_offset+2], p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
}

const BL_1c128_t* BL_styz_sy(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset)
{
    // work[0] = U - S, work[1] = U + S
    matfnc->sub(p->IMat, s, p->work[index_offset+0], p->port_count, p->port_count);
    matfnc->add(p->IMat, s, p->work[index_offset+1], p->port_count, p->port_count);
    // work[2] = inv(work[1])
    matfnc->inv(p->work[index_offset+1], p->work[index_offset+2], p->work[index_offset+3], p->port_count);
    // work[1] = work[0] * work[2]
    matfnc->mul(p->work[index_offset+0], p->work[index_offset+2], p->work[index_offset+1], p->port_count, p->port_count, p->port_count);
    // work[2] = Sy * work[1], work[0] = work[2] * Sy
    matfnc->mul(p->Sy, p->work[index_offset+1], p->work[index_offset+2], p->port_count, p->port_count, p->port_count);
    return matfnc->mul(p->work[index_offset+2], p->Sy, p->work[index_offset+0], p->port_count, p->port_count, p->port_count);
}

/*!
borrow work area from *p for workq
\param p [in] linear circuit parameter accumlator exporting the work area
\param index_begin [in] index of work matrix beginning in *p.
\param ppworkq [out] array of pointer pointing each matrix of quarter size
\param matcount_workq [in] number of quarter sized matrices
\return error check code
*/
static int BL_styz_qmattool(pBL_styz_t p, uint32_t index_begin, BL_1c128_t **ppworkq, uint32_t matcount_workq)
{
    int err = ESUCCESS;
    do {
        if (((p->port_count & 1) != 0) // odd number port count is not allowed.
            || ((matcount_workq & 3) != 0)) // matcout of quarter size matrix other than 4N is not allowed.
        { 
            err = EINVAL;
            break;
        }
        uint32_t ce_workq = (p->port_count * p->port_count)/4;
        for (uint32_t i = 0; i != matcount_workq; i++)
        {
            ppworkq[i] = p->work[index_begin] + ce_workq * i;
        }
    } while (0);
    return err;
}

const BL_1c128_t* BL_styz_ts(pBL_styz_t p, const BL_1c128_t* t, uint32_t index_offset)
{
    int err = ESUCCESS;
    do {
        BL_1c128_t* workq[12];
        if (ESUCCESS != (err = BL_styz_qmattool(p, index_offset, workq, ARRAYSIZE(workq))))
        {
            break;
        }
        uint32_t qmat_size = p->port_count/2;
        // workq[0]=TUL, workq[1]=TUR, workq[2]=TLL, workq[3]=TLR
        matfnc->getsubmat(t, workq[0], p->port_count, p->port_count, qmat_size, qmat_size, 0, 0);
        matfnc->getsubmat(t, workq[1], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, 0);
        matfnc->getsubmat(t, workq[2], p->port_count, p->port_count, qmat_size, qmat_size, 0, qmat_size);
        matfnc->getsubmat(t, workq[3], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, qmat_size);

        // SLL(=workq[6])=inv(TLR)=inv(workq[3]), workq[8,9]:work matrix for inv()
        matfnc->inv(workq[3], workq[6], workq[8], qmat_size);
        // SUL(workq[4]) = TUR * SLL = workq[1] * workq[6]
        matfnc->mul(workq[1], workq[6], workq[4], qmat_size, qmat_size, qmat_size);
        // SLR(workq[7]) = -SLL * TLL, work[8] = SLL * TLL
        matfnc->mul(workq[6], workq[2], workq[8], qmat_size, qmat_size, qmat_size);
        matfnc->fill(CMPLX(0.0,0.0), workq[9], qmat_size, qmat_size);
        matfnc->sub(workq[9], workq[8], workq[7], qmat_size, qmat_size);
        // SUR(workq[5]) = TUL + TUR * SLR = workq[0] + workq[1] * workq[7]
        matfnc->mul(workq[1], workq[7], workq[9], qmat_size, qmat_size, qmat_size);
        matfnc->add(workq[0], workq[9], workq[5], qmat_size, qmat_size);
        // merge and move workq[4]..workq[7] to p->work[index_offset]
        matfnc->setsubmat(p->work[index_offset], workq[4], p->port_count, p->port_count, qmat_size, qmat_size, 0, 0);
        matfnc->setsubmat(p->work[index_offset], workq[5], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, 0);
        matfnc->setsubmat(p->work[index_offset], workq[6], p->port_count, p->port_count, qmat_size, qmat_size, 0, qmat_size);
        matfnc->setsubmat(p->work[index_offset], workq[7], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, qmat_size);
    } while (0);
    return err ? NULL : p->work[index_offset];
}

const BL_1c128_t* BL_styz_st(pBL_styz_t p, const BL_1c128_t* s, uint32_t index_offset)
{
    int err = ESUCCESS;
    do {
        BL_1c128_t* workq[12];
        if (ESUCCESS != (err = BL_styz_qmattool(p, index_offset, workq, ARRAYSIZE(workq))))
        {
            break;
        }
        uint32_t qmat_size = p->port_count/2;
        // workq[0]=SUL, workq[1]=SUR, workq[2]=SLL, workq[3]=SLR
        matfnc->getsubmat(s, workq[0], p->port_count, p->port_count, qmat_size, qmat_size, 0, 0);
        matfnc->getsubmat(s, workq[1], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, 0);
        matfnc->getsubmat(s, workq[2], p->port_count, p->port_count, qmat_size, qmat_size, 0, qmat_size);
        matfnc->getsubmat(s, workq[3], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, qmat_size);

        // TLR(workq[7]) = inv(SLL) = inv(workq[2]), work matrices: workq[4,5]
        matfnc->inv(workq[2], workq[7], workq[4], qmat_size);
        // TUR(workq[5]) = SUL * TLR = workq[0] * workq[7]
        matfnc->mul(workq[0], workq[7], workq[5], qmat_size, qmat_size, qmat_size);
        // TLL(workq[6]) = -TLR * SLR = -workq[7] * workq[3]
        matfnc->mul(workq[7], workq[3], workq[4], qmat_size, qmat_size, qmat_size);
        matfnc->muls(workq[4], qmat_size, qmat_size, CMPLX(-1.0, 0.0), workq[6]);
        // TUL(workq[4]) = SUR + SUL * TLL = workq[1] + workq[0] * workq[6]
        matfnc->mul(workq[0], workq[6], workq[8], qmat_size, qmat_size, qmat_size);
        matfnc->add(workq[1], workq[8], workq[4], qmat_size, qmat_size);
        // merge and move workq[4]..workq[7] to p->work[index_offset]
        matfnc->setsubmat(p->work[index_offset], workq[4], p->port_count, p->port_count, qmat_size, qmat_size, 0, 0);
        matfnc->setsubmat(p->work[index_offset], workq[5], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, 0);
        matfnc->setsubmat(p->work[index_offset], workq[6], p->port_count, p->port_count, qmat_size, qmat_size, 0, qmat_size);
        matfnc->setsubmat(p->work[index_offset], workq[7], p->port_count, p->port_count, qmat_size, qmat_size, qmat_size, qmat_size);
    } while (0);
    return err ? NULL : p->work[index_offset];
}
