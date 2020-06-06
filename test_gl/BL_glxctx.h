#ifndef BL_GLXCTX_H_
#define BL_GLXCTX_H_
#include    "test_gl/BL_glctx.h"
#include    <X11/Xlib.h>
#include    <X11/Xutil.h>
#include    <GL/gl.h>
#include    <GL/glx.h>

#ifdef  __cplusplus
extern  "C" {
#endif
typedef struct
{
    BL_glctx_t base;
    // window-by-window data
    Window win;
    BL_1r32_t   mvp[16], mproj[16], mmv[16]; // viewport, projection, and model-view matrix
} BL_glxctx_t, *pBL_glxctx_t;

typedef const BL_glxctx_t *pcBL_glxctx_t;

/*!
\brief create an OpenGL context and load core APIs bound to GLX
*/
pBL_glxctx_t    BL_glxctx_new(const BL_2u32_t wh, const char* name);

/*!
\brief delete the OpenGL context
*/
void    BL_glxctx_delete(pBL_glxctx_t* ppobj);


/*!
\brief initialize the OpenGL context
*/
int     BL_glxctx_init(pBL_glxctx_t ctx, const void* params);

/*!
\brief clear the framebuffer
*/
int     BL_glxctx_clear(pBL_glxctx_t ctxp);

/*!
\brief draw in the framebuffer
*/
int     BL_glxctx_draw(pBL_glxctx_t ctx, BL_glctx_drawfunc draw_func, const void* obj3D);

/*!
\brief flush the drawn image to show in display
*/
int     BL_glxctx_flush(pBL_glxctx_t ctx, pBL_arrayMD_t* ppimage);

#ifdef  __cplusplus
}
#endif
#endif