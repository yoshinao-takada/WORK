#include    "test_gl/BL_glxctx.h"
#include    "test_gl/BL_glxcommon.h"
#include    <stdbool.h>
#include    <errno.h>
#include    <string.h>
#include    <stdio.h>
#include    <unistd.h>

pBL_glxctx_t    BL_glxctx_new(const BL_2u32_t wh, const char* name)
{
    pBL_glxctx_t p = NULL;
    Display *display = BL_glxcommon_get()->display;
    XSetWindowAttributes swa = BL_glxcommon_get()->swa;
    XVisualInfo* vi = BL_glxcommon_get()->vi;
    int err = ESUCCESS;
    do {
        p = BL_CALLOC(1,BL_glxctx_t);
        BL_copy2(p->base.wh, wh);
        p->base.name = strdup(name);
        // create a window and cleanup the visual
        p->win = XCreateWindow(display, RootWindow(display, vi->screen), 
            0,0, (int)p->base.wh[0], (int)p->base.wh[1], 0, vi->depth, InputOutput,
            vi->visual, 
            CWBorderPixel | CWColormap | CWEventMask, &swa);
        if (!(p->win))
        {
            err = EFAULT;
        }
        XStoreName(display, p->win, p->base.name);
        XMapWindow(display, p->win);
        glXMakeCurrent(display, p->win, BL_glxcommon_get()->ctx);
    } while (false);
    if (err) { BL_SAFEFREE(&p); }
    return p;
}

void    BL_glxctx_delete(pBL_glxctx_t* ppobj)
{
    pBL_glxctx_t glxctx = *ppobj;
    int status = XDestroyWindow(BL_glxcommon_get()->display, glxctx->win);
    if (status == 0)
    {
        printf("Fail in XDestroyWindow().\n");
    }
    *ppobj = NULL;
}

int     BL_glxctx_clear(pBL_glxctx_t ctx)
{
    int err = ESUCCESS;
    Display* display = BL_glxcommon_get()->display;
    do {
        glClearColor( 0, 0.5, 1, 1 );
        glClear( GL_COLOR_BUFFER_BIT );
        glXSwapBuffers ( display, ctx->win );

        sleep( 1 );

        glClearColor ( 1, 0.5, 0, 1 );
        glClear ( GL_COLOR_BUFFER_BIT );
        glXSwapBuffers ( display, ctx->win );

        sleep( 1 );
    } while (false);
    return err;
}

int     BL_glxctx_init(pBL_glxctx_t ctx, const void* params)
{
    int err = ESUCCESS;
    do {

    } while (false);
    return err;
}

int     BL_glxctx_draw(pBL_glxctx_t ctx, BL_glctx_drawfunc draw_func, const void* obj3D)
{
    return draw_func(&ctx->base, obj3D);
}

int     BL_glxctx_flush(pBL_glxctx_t ctx, pBL_arrayMD_t* ppimage)
{
    int err = ESUCCESS;
    do {
        BL_arrayMD_dims dims = { (uint16_t)ctx->base.wh[0], (uint16_t)ctx->base.wh[1], 1,1,1,1,1,1 };
        *ppimage  =BL_arrayMD_new(dims, BL_3u8_t);
        BL_ptr_t ptr = BL_arrayMD_begin(*ppimage);
        glReadPixels(0,0,(GLsizei)dims[0], (GLsizei)dims[1], GL_BGR, GL_UNSIGNED_BYTE, ptr._1u8);
        glXSwapBuffers(BL_glxcommon_get()->display, ctx->win);
    } while (false);
    return err;
}