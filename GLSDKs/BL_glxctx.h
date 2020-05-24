#ifndef BL_GLXCTX_H_
#define BL_GLXCTX_H_
#include    "GLSDKs/BL_glctx.h"
#include    <X11/X.h>
#include    <X11/Xlib.h>
#include    <X11/Xutil.h>
#include    <GL/gl.h>
#include    <GL/glx.h>
// https://www.khronos.org/opengl/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib
// https://www.khronos.org/opengl/wiki/Tutorial:_OpenGL_3.0_Context_Creation_(GLX)
#ifdef __cplusplus
extenr "C" {
#endif

typedef struct {
    BL_glxctx_t base;
    Display* dpy;
    Window root, win;
    Colormap cmap;
    XSetWindowAttributes xswa;
} BL_glxctx_h, *pBL_glxctx_h;
typedef const BL_glxctx_h *pcBL_glxctx_h;

#define BL_GLX_VISUAL_ATTRIBUTES   { \
    { GLX_X_RENDERABLE, True }, \
    { GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT }, \
    { GLX_RENDER_TYPE, GLX_RGBA_BIT }, \
    { GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR }, \
    { GLX_RED_SIZE, 8 }, \
    { GLX_GREEN_SIZE, 8 }, \
    { GLX_BLUE_SIZE, 8 }, \
    { GLX_ALPHA_SIZE, 8 }, \
    { GLX_DEPTH_SIZE, 24 }, \
    { GLX_STENCIL_SIZE, 8 }, \
    { GLX_DOUBLEBUFFER, True }, \
    { GLX_SAMPLE_BUFFERS, 1 }, \
    { GLX_SAMPLES, 4 }, \
    { None, 0 } \
}

/*!
\brief create a new object but NOT bind a new window yet.
*/
pBL_glxctx_t BL_glxctx_new(const BL_2i32_t xy, const BL_2u32_t wh, const BL_4r32_t clear_color, const char* window_caption);


/*!
\brief bind a physical window served by X-Window, Windows, etc.
*/
void BL_glxctx_bind_window(pBL_glxctx_t ctx);

/*!
\brief update geometrical parameters with the current status of the bound window.
*/
void BL_glxctx_update_geometry(pBL_glxctx_t ctx);

/*!
\brief clear the current swap buffer to render a new image.
*/
void BL_glxctx_clear_image(pBL_glxctx_t ctx);

/*!
\brief render a new image
\param ctx [in,out] rendering context
\param render_callback [in] rendering function
\param data [in] rendering data
\return render_callback return code
*/
int BL_glxctx_render(pBL_glxctx_t ctx, BL_GLCTX_RENDERCALLBACK render_callback, const void* data);

/*!
\brief retrieve the latest image and swap buffer.
\param ctx [in,out] rendering context
\param wh [out] image size
\param pixbuf [out] B8G8R8 buffer in row major layout; if wh == NULL || pixbuf == NULL, don't retrieve the image.
*/
int BL_glxctx_flushout(pBL_glxctx_t ctx, BL_2u32_t wh, void** pixbuf);
#ifdef __cplusplus
}
#endif
#endif /* BL_GLXCTX_H_ */