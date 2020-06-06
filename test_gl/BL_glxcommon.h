#ifndef BL_GLXCOMMON_H_
#define BL_GLXCOMMON_H_
#include    <X11/Xlib.h>
#include    <X11/Xutil.h>
#include    <GL/gl.h>
#include    <GL/glx.h>
#include    "base_l/BL_base.h"
#include    "base_l/BL_types.h"

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext  (*glXCreateContextAttribARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#ifdef  __cplusplus
extern "C" {
#endif
typedef struct {
    BL_2i32_t   glx_version; // [0]:major, [1]:minor
    Display *display;
    GLXFBConfig bestFbc;
    XSetWindowAttributes swa;
    Colormap    cmap;
    XVisualInfo* vi;
    GLXContext  ctx;
} BL_glxcommon_t, *pBL_glxcommon_t;
typedef const BL_glxcommon_t *pcBL_glxcommon_t;

/*!
\brief init GLX common context parameters
*/
int BL_glxcommon_init();

/*!
\brief get a pointer to the common context parameter.
*/
pBL_glxcommon_t BL_glxcommon_get();

/*!
\brief cleanup the common context parameters
*/
void BL_glxcommon_cleanup();

#define VISUAL_ATTRIB_DEFS  { \
    GLX_X_RENDERABLE    , True, \
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT, \
    GLX_RENDER_TYPE     , GLX_RGBA_BIT, \
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR, \
    GLX_RED_SIZE        , 8, \
    GLX_GREEN_SIZE      , 8, \
    GLX_BLUE_SIZE       , 8, \
    GLX_ALPHA_SIZE      , 8, \
    GLX_DEPTH_SIZE      , 24, \
    GLX_STENCIL_SIZE    , 8, \
    GLX_DOUBLEBUFFER    ,   True, \
    None \
}

#ifdef  __cplusplus
}
#endif
#endif