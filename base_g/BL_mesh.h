#ifndef DATA3D_H_
#define DATA3D_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
// types of vertex indices
typedef enum {
    BL_vertidx_triangle, // triangle corners
    BL_vertidx_tristrip, // triangle strip
    BL_vertidx_trifan, // triangle fan
    BL_vertidx_edge_end_pair, // pair of edge ends
    BL_vertidx_polyline,
    BL_vertidx_discrete_points,
    BL_vertidx_tetrahedron,
} BL_vertidxtype_t, *pBL_vertidxtype_t;
typedef const BL_vertidxtype_t *pcBL_vertidxtype_t;

#define BL_VERTIDXSIZES  { 3, 1, 1, 2, 1, 1, 4 }

/*!
\brief It is a configurable container of 3D mesh, either surface mesh or solid mesh.
\param vert: vertex attributes, primarily coordinate, secondarily texture coordinate or color, ...
\param indices: it is an extensible array of anchors of versatile types of linkable array.
*/
typedef struct {
    pBL_arrayMD_t vert;
    BL_linkable_array_t indices[1];
} BL_mesh_t, *pBL_mesh_t;
typedef const BL_mesh_t *pcBL_mesh_t;

/*!
\brief create a new object of BL_mesh_t.
\param vertWH [in] 2D size of vert. vertWH[0]:width, vertWH[1]: height, vertWH[2..7]:must be 1.
\param n_indices [in] indices count
\param vertidxtypes [in]
\return a pointer to newly created object
*/
pBL_mesh_t  BL_mesh_alloc(const BL_arrayMD_dims vertWH, uint32_t cb_vert_element, uint32_t n_indices, pcBL_vertidxtype_t vertidxtypes);
#define BL_mesh_new(vertWH, unit_type, n_indices, vertidxtypes) BL_mesh_alloc(vertWH, sizeof(unit_type), n_indices, vertidxtypes)

/*!
\brief delete an object.
*/
void    BL_mesh_delete(pBL_mesh_t* ppmesh);

/*
\brief get an element count of each index types. i.e. triangle:3, edge_end_pair:2, discrete points:1...
They are defined as BL_VERTIDXSIZES correponding to the declaration of BL_vertidxtype_t.
*/
uint32_t BL_mesh_vertidxsize(BL_vertidxtype_t type);
#ifdef __cplusplus
}
#endif
#endif
