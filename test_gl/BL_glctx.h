#ifndef BL_GLCTX_H_
#define BL_GLCTX_H_
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"
#include    "base_l/BL_containers.h"

#ifdef __cplusplus
extern "C" {
#endif
// incomplete object which BL_glxctx_t inherits.
typedef struct {
    BL_2u32_t wh; // rendering window size
    char* name;
} BL_glctx_t, *pBL_glctx_t;

typedef const BL_glctx_t *pcBL_glctx_t;

/*!
\brief create an OpenGL context and load core APIs
*/
pBL_glctx_t BL_glctx_new(const BL_2u32_t wh, const char* name);

/*!
\brief delete the OpenGL context
*/
void    BL_glctx_delete(pBL_glctx_t* ppobj);

/*!
\brief initialize the OpenGL context
*/
int     BL_glctx_init(pBL_glctx_t ctx, const void* params);

/*!
\brief clear the framebuffer
*/
int     BL_glctx_clear(pBL_glctx_t ctx);

typedef int (*BL_glctx_drawfunc)(pBL_glctx_t ctx, const void* obj3D);
/*!
\brief draw in the framebuffer
*/
int     BL_glctx_draw(pBL_glctx_t ctx, BL_glctx_drawfunc draw_func, const void* obj3D);

/*!
\brief flush the drawn image to show in display
*/
int     BL_glctx_flush(pBL_glctx_t ctx, pBL_arrayMD_t* ppimage);


#ifdef __cplusplus
}
#endif
#endif
