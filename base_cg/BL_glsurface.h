#ifndef BL_GLSURFACE_H_
#define BL_GLSURFACE_H_
#include    <GL/gl.h>
#include    <base_l/BL_base.h>
#include    <base_l/BL_types.h>
#ifdef  __cplusplus
extern "C" {
#endif
    typedef struct {
        BL_4r32_t* xyzw; // spatial coordinate
        BL_2r32_t* uv; // texture coordinate
        uint32_t n_vert; // vertex count
    } BL_vert_t, *pBL_vert_t;
    typedef const BL_vert_t *pcBL_vert_t;

    typedef struct {
        int* vert_indices;
        GLenum type; // GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, etc.
        uint32_t n_int; // number of integers
    } BL_indices_t, *pBL_indices_t;
    typedef const BL_indices_t *pcBL_indices_t;
    
#ifdef  __cplusplus
}
#endif
#endif