#include    "base_l/BL_debutil.h"

void BL_debutil_write_2r32(FILE* pf, BL_cptr_t* ptr)
{
    fprintf(pf, ",%f,%f", ptr->_1r32[0], ptr->_1r32[1]);
    ptr->_2r32++;
}

void BL_debutil_write_1u16(FILE* pf, BL_cptr_t* ptr)
{
    fprintf(pf, ",%u", *ptr->_1u16++);
}

void BL_debutil_write_1u32(FILE* pf, BL_cptr_t* ptr)
{
    fprintf(pf, ",%u", *ptr->_1u32++);
}

void BL_debutil_fwrite_array2D(
    FILE* pf, pcBL_arrayMD_t array, const char* header, BL_DEBUTIL_FWRITE_ELEMENT element_writer)
{
    fprintf(pf, "%s\n", header);
    BL_cptr_t ptr = BL_arrayMD_cbegin(array);
    for (int16_t iy = 0; iy != array->dims[1]; iy++)
    {
        for (int16_t ix = 0; ix != array->dims[0]; ix++)
        {
            fprintf(pf, "%d,%d", iy, ix);
            element_writer(pf, &ptr);
            fprintf(pf, "\n");
        }
    }
}

void BL_debutil_fwrite_array(
    FILE* pf, pcBL_array_t array, const char* header, BL_DEBUTIL_FWRITE_ELEMENT element_writer)
{
    fprintf(pf, "%s\n", header);
    BL_cptr_t ptr = BL_array_cbegin(array);
    for (uint32_t i = 0; i != array->unit_count; i++)
    {
        fprintf(pf, "%d", i);
        element_writer(pf, &ptr);
        fprintf(pf, "\n");
    }
}