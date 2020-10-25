#include    "base_z/BL_snparray.h"
#include    "base_l/BL_containers.h"
#include    <math.h>

pBL_snparray_t BL_snparray_new(uint32_t points, uint32_t ports)
{
    pBL_snparray_t p = NULL;
    int err = ESUCCESS;
    do {
        size_t cb_head = sizeof(BL_snparray_t);
        size_t cb_extra = sizeof(BL_1c128_t) * ((ports * ports + 1) * points + ports);
        if (NULL == (p = (pBL_snparray_t)malloc(cb_head + cb_extra)))
        {
            err = ENOMEM;
            break;
        }
        p->type = BL_snparray_type_UNKNOWN;
        p->points = points;
        p->ports = ports;
        p->element_size = ports * ports + 1;
        p->zc = p->data;
        p->elements = p->zc + ports;
    } while (0);
    if (err != ESUCCESS && p)
    {
        BL_SAFEFREE(&p);
    }
    return p;
}

void BL_snparray_setZc(pBL_snparray_t a, BL_1c128_t zc)
{
    for (uint32_t i = 0; i != a->ports; i++)
    {
        a->zc[i] = zc;
    }
}

void BL_snparray_setZc2(pBL_snparray_t a, const BL_1c128_t* zc)
{
    BL_copy_array(a->zc, zc, a->ports);
}

static int equal_size(pcBL_snparray_t a0, pcBL_snparray_t a1)
{
    return ((a0->ports == a1->ports) && (a0->points == a1->points)) ?
        ESUCCESS : ERR_MATRIX_SIZEMISMATCH;
}

static int equal_ports(pcBL_snparray_t a0, pcBL_snparray_t a1)
{
    return (a0->ports == a1->ports) ? ESUCCESS : ERR_MATRIX_SIZEMISMATCH;
}

int BL_snparray_copy(pBL_snparray_t dst, pcBL_snparray_t src)
{
    int err = ESUCCESS;
    do {
        if (ESUCCESS != (err = equal_size(dst, src)))
        {
            break;
        }
        BL_copy_array(dst->data, src->data, dst->points * dst->element_size + dst->ports);
    } while (0);
    return err; 
}

int BL_snparray_partial_copy(
    pBL_snparray_t dst, pcBL_snparray_t src, uint32_t dst_begin, uint32_t src_begin, uint32_t length)
{
    int err = ESUCCESS;
    do {
        if (ESUCCESS != (err = equal_ports(dst, src)))
        {
            break;
        }
        uint32_t dst_end = dst_begin + length, src_end = src_begin + length;
        if (dst_end > dst->points || src_end > src->points)
        {
            err = ERANGE;
            break;
        }
        BL_copy_array(dst->elements + dst_begin * dst->element_size,
            src->elements + src_begin * src->element_size,
            src->element_size * length);
    } while (0);
    return err;
}

int BL_snparray_set_freq(pBL_snparray_t dst, double freq, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= dst->points)
        {
            err = ERANGE;
            break;
        }
        BL_1c128_t* element = dst->elements + dst->element_size * index;
        element[0] = CMPLX(2.0 * M_PI * freq, 0.0);
    } while (0);
    return err;
}


int BL_snparray_set_angularfreq(pBL_snparray_t dst, double angularfreq, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= dst->points)
        {
            err = ERANGE;
            break;
        }
        BL_1c128_t* element = dst->elements + dst->element_size * index;
        element[0] = CMPLX(angularfreq, 0.0);
    } while (0);
    return err;
}

int BL_snparray_set_mat(pBL_snparray_t dst, const BL_1c128_t* mat, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= dst->points)
        {
            err = ERANGE;
            break;
        }
        BL_1c128_t* dst_mat = dst->elements + dst->element_size * index + 1;
        BL_copy_array(dst_mat, mat, dst->element_size-1);
    } while (0);
    return err;
}

int BL_snparray_set_mat_element(
    pBL_snparray_t dst, BL_1c128_t element, uint32_t index, uint32_t row, uint32_t col)
{
    int err = ESUCCESS;
    do {
        if ((index >= dst->points) || (row >= dst->ports) || (col >= dst->ports))
        {
            err = ERANGE;
            break;
        }
        uint32_t in_mat_offset = col + row * dst->ports;
        *(dst->elements + dst->element_size * index + 1 + in_mat_offset) = element;
    } while (0);
    return err;
}

int BL_snparray_get_freq(pcBL_snparray_t src, double* freq, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= src->points)
        {
            err = ERANGE;
            break;
        }
        const BL_1c128_t *element = src->elements + src->element_size * index;
        *freq = creal(*element)/(2.0 * M_PI);
    } while (0);
    return err;
}

int BL_snparray_get_angularfreq(pcBL_snparray_t src, double* angularfreq, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= src->points)
        {
            err = ERANGE;
            break;
        }
        const BL_1c128_t *element = src->elements + src->element_size * index;
        *angularfreq = creal(*element);
    } while (0);
    return err;
}

int BL_snparray_get_mat(pcBL_snparray_t src, BL_1c128_t *mat, uint32_t index)
{
    int err = ESUCCESS;
    do {
        if (index >= src->points)
        {
            err = ERANGE;
            break;
        }
        const BL_1c128_t* src_mat_begin = src->elements + src->element_size * index + 1;
        BL_copy_array(mat, src_mat_begin, src->element_size -1)
    } while (0);
    return err;
}

int BL_snparray_get_mat_element(
    pcBL_snparray_t src, BL_1c128_t *element, uint32_t index, uint32_t row, uint32_t col)
{
    int err = ESUCCESS;
    do {
        if (index >= src->points)
        {
            err = ERANGE;
            break;
        }
        uint32_t in_mat_offset = col + row * src->ports;
        const BL_1c128_t *src_element = src->elements + index * src->element_size + 1 + in_mat_offset;
        *element = *src_element;
    } while (0);
    return err;
}


