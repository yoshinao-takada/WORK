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
        uint32_t n_vert, n_tri; // vertex count and triangle count
        BL_3r32_t* vert; // vertex coordinates
        BL_2r32_t* tex; // texture coordinates
        BL_1u8_t*  flags; // combination of BL_oomesh_attr_t
        BL_3i32_t* tri; // triangle vertex indeices
    } BL_oomesh_t, *pBL_oomesh_t;

    typedef const BL_oomesh_t *pcBL_oomesh_t;

    /*!
    \brief create a new mesh as a 2-D organized mesh
    */
    pBL_oomesh_t BL_oomesh_new(const BL_2u32_t wh_);

    /*!
    \brief create a new mesh as a 1-D mesh
    */
    pBL_oomesh_t BL_oomesh_new2(uint32_t n_vert, uint32_t n_tri);

    /*!
    \brief triangulate in default manner
    \param mesh [in,out]
    */
    void BL_oomesh_setdefaulttriangles(pBL_oomesh_t mesh);

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

    /*!
    \brief get only enabled vertices
    \param mesh_in [in] mesh with disabled points
    \param mesh_out [out] mesh without disabled points. Only enabled points in mesh_in are copied.
    mesh_out->wh becomes having no meaning and wh is set to { point_count, 1 }. In the case that
    *mesh_out is not null at entering the function, free(*mesh_out) is called in the function
    to free the memory block.
    */
    void BL_oomesh_extractenabled(pcBL_oomesh_t mesh_in, pBL_oomesh_t* mesh_out);

#pragma region accessor_related_methods
    typedef struct {
        BL_3r32_t *vert;
        BL_2r32_t *tex;
        uint8_t *flags;
    } BL_oomeshpointinfo_t, *pBL_oomeshpointinfo_t;

    typedef const BL_oomeshpointinfo_t *pcBL_oomeshpointinfo_t;

    void BL_oomeshpointinfo_gotonext(pBL_oomeshpointinfo_t info);

    /*!
    \brief get a vertex iterator
    */
    BL_oomeshpointinfo_t BL_oomesh_begin(pBL_oomesh_t mesh);

    typedef struct {
        const BL_3r32_t *vert;
        const BL_2r32_t *tex;
        const uint8_t *flags;
    } BL_oomeshconstpointinfo_t, *pBL_oomeshconstpointinfo_t;

    typedef const BL_oomeshconstpointinfo_t *pcBL_oomeshconstpointinfo_t;

    void BL_oomeshconstpointinfo_gotonext(pBL_oomeshconstpointinfo_t info);

    /*!
    \brief get a const vertex iterator
    */
    BL_oomeshconstpointinfo_t BL_oomesh_cbegin(pcBL_oomesh_t mesh);

    void BL_oomeshpointinfo_copy(pcBL_oomeshpointinfo_t dst, pcBL_oomeshconstpointinfo_t src);
#pragma endregion
#ifdef __cplusplus
}
#endif
#endif
