#ifndef BL_OOMESH_H_
#define BL_OOMESH_H_
#include    <base_l/BL_containers.h>
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum {
        BL_oomesh_attr_enabled = 1,
    } BL_oomesh_attr_t;

    typedef struct {
        BL_2u32_t  wh; // width and height
        // The following three arrays has wh[0]*wh[1] elements and treated as (column,row) 2D arrays.
        // The serial index is defined as column + row * wh[0], column = [0..wh[0]),
        // row = [0..wh[1])
        BL_3r32_t* vert; // vertex coordinates
        BL_2r32_t* tex; // texture coordinates
        BL_1u8_t*  flags; // combination of BL_oomesh_attr_t
    } BL_oomesh_t, *pBL_oomesh_t;

    typedef const BL_oomesh_t *pcBL_oomesh_t;

    pBL_oomesh_t BL_oomesh_new(const BL_2u32_t wh_);

    /*!
    \brief apply a mask of BGR image. BL_oomesh_attr_enabled bit is reset if
        mask's BGR all color channels == 0.
    \param mesh [in,out]
    \param mask [in]
    */
    void BL_oomesh_applymask(pBL_oomesh_t mesh, const BL_3u8_t* mask);

    /*!
    \brief set texture coordinates to uniform grid
    \param mesh [in,out]
    \param tex0 [in] texture coordinate corresponding to mesh vertex array top-left element
    \param tex1 [in] texture coordinate corresponding to mesh vertex array bottom-right element
    */
    void BL_oomesh_settex(pBL_oomesh_t mesh, const BL_2r32_t tex0, const BL_2r32_t tex1);

    /*!
    \brief set uniform grid vertex coordinate
    \param mesh [in,out]
    \param vert0 [in] vertex coordinate corresponding to mesh vertex array top-left element
    \param vert1 [in] vertex coordinate corresponding to mesh vertex array bottom-right element
    */
    void BL_oomesh_setvert(pBL_oomesh_t mesh, const BL_3r32_t vert0, const BL_3r32_t vert1);
#ifdef __cplusplus
}
#endif
#endif
