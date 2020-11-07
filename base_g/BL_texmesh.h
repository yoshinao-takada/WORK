#ifndef BL_TEXMESH_H_
#define BL_TEXMESH_H_
#include    <base_l/BL_base.h>
#include    <base_l/BL_types.h>
#include    <base_l/BL_containers.h>
#include    <base_g/BL_3Dline.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    // vertex consists of 3D coordinate, texture image No., and normalized texture coordinate
    // array of BL_6r32_t
    pBL_array_t vert;
    // vertex indices of triangles, array of BL_3i32_t 
    pBL_array_t tri;
} BL_texmesh_t, *pBL_texmesh_t;
typedef const BL_texmesh_t *pcBL_texmesh_t;

/*!
\brief initialize a mesh
\param mesh [in,out] empty mesh, which has mesh->vert = mesh->tri = NULL
\param n_vert [in] number of vertices
\param vert [in] vertex data, vert[6*n_vert]
\param n_tri [in] number of triangles
\param tri [in] vertex index of triangle corners. tri[3*n_tri]
*/
int BL_texmesh_init(pBL_texmesh_t mesh, int n_vert, const BL_1r32_t* vert, int n_tri, const BL_1i3_t* tri);

/*!
\brief create a new mesh object
\param n_vert [in] number of vertices
\param vert [in] vertex data, vert[6*n_vert]
\param n_tri [in] number of triangles
\param tri [in] vertex index of triangle corners. tri[3*n_tri]
\param ppmesh [out]
\return errno compatible number
*/
int BL_texmesh_new(int n_vert, const BL_1r32_t* vert, int n_tri, const BL_1i3_t* tri, pBL_texmesh_t*ppmesh);

/*!
\brief delete an existing object created by BL_texmesh_new().
*/
void BL_texmesh_delete(pBL_texmesh_t* ppmesh);

/*!
\brief get the texture image No. and normalized texture coordinate
\param mesh [in] textured mesh
\param line [in] view line
\param n_image [out] image No.
\param tex_coord [out] texture coord
\return ESUCCESS: a triangle which the line go through was found. ENODATA: no triangle was found on the view line
*/
int BL_texmesh_see(pcBL_texmesh_t mesh, pcBL_3Dline_t line, int* n_image, BL_2r32_t tex_coord);
#ifdef __cplusplus
}
#endif
#endif /* BL_TEXMESH_H_ */