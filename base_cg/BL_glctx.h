#ifndef BL_GLCTX_H_
#define BL_GLCTX_H_
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <base_l/BL_types.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
    // maximum available windows for OpenGL
    #define BL_GLWINDOWS_NUM    8

    struct BL_glctx;
    // The callback functions get arguments from the global context g_glctx.
    // Therefore the function signature has no arguments.
    typedef int (*BL_GLCTX_CALLBACK)();
    int BL_GLCTX_donothing();

    typedef struct {
        BL_GLCTX_CALLBACK   drawscene;
        BL_GLCTX_CALLBACK   chgwindow;
        BL_GLCTX_CALLBACK   movewindow;
        BL_GLCTX_CALLBACK   keypressed, keyreleased;
        BL_GLCTX_CALLBACK   mousebuttonpressed, mousebuttonreleased;
    } BL_glctxcallbacks_t, *pBL_glctxcallbacks_t;
    typedef const BL_glctxcallbacks_t* pcBL_glctxcallbacks_t;
    #define BL_GLCTXCALLBACK_DEF    { \
        BL_GLCTX_donothing, BL_GLCTX_donothing, BL_GLCTX_donothing, \
        BL_GLCTX_donothing, BL_GLCTX_donothing, BL_GLCTX_donothing, BL_GLCTX_donothing }

    void BL_glctxcallbacks_copy(pBL_glctxcallbacks_t dst, pcBL_glctxcallbacks_t src);
    typedef struct {
        BL_2i32_t position;
        BL_2u32_t size;
        char    name[0];
    } BL_windowinfo_t, *pBL_windowinfo_t;
    typedef const BL_windowinfo_t* pcBL_windowinfo_t;

    typedef struct BL_glwininfo {
        Window  win;
        GLXContext  glc;
        XWindowAttributes   gwa;

        // my own attributes
        BL_windowinfo_t wi;

        // textured mesh related attributes
        int gl_texture; // texture tag initialized by glGenTextures()
        uint32_t n_vert; // vertex count of those registered by BL_glutils_settexturedmesh()

        // window specific callback functions
        BL_glctxcallbacks_t callbacks;
    } BL_glwininfo_t, *pBL_glwininfo_t;
    typedef const BL_glwininfo_t *pcBL_glwininfo_t;

    typedef struct BL_glctx {
        Display* dpy;
        Window  root;
        GLint   att[5];
        XVisualInfo*    vi;
        Colormap    cmap;
        XSetWindowAttributes    swa;
        uint32_t i_current; // index to the active element in win[]. no element is active if i_current == BL_GLWINDOWS_NUM.
        XEvent      xev;
        bool        b_continue;
        pBL_glwininfo_t win[BL_GLWINDOWS_NUM];
    } BL_glctx_t, *pBL_glctx_t;

    typedef const BL_glctx_t* pcBL_glctx_t;

    /*!
    \brief init context with defaults
    \param i_window [in] window index
    \param name [in] window name
    \param win_pos [in] initial window position
    \param win_size [in] initial window size
    \return unix errno
    */
    int BL_glctx_new(uint32_t i_window, const char* name, const BL_2i32_t win_pos, const BL_2u32_t win_size);

    /*!
    \brief delete a contex
    \param i_window [in] window index
    \return unix errno
    */
    int BL_glctx_delete(uint32_t i_window);

    /*!
    \brief select a context
    \param i_window [in] window index
    \return unix errno
    */
    int BL_glctx_select(uint32_t i_window, pBL_glwininfo_t *ppwin);

    /*!
    \brief set event handler callback functions
    */
    int BL_glctx_setcallbacks(uint32_t i_window, pcBL_glctxcallbacks_t callbacks);

    /*!
    \brief get event handler callback functions
    */
    int BL_glctx_getcallbacks(uint32_t i_window, pBL_glctxcallbacks_t callbacks);

    /*!
    \brief find the index in the context for argument win.
    \return the index of win or BL_GLWINDOWS_NUM (is the array size of the window contexts).
    */
    uint32_t BL_glctx_findwindow(Window win);
#ifdef __cplusplus
}
#endif
#endif