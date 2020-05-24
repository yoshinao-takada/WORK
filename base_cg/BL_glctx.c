#include    "base_cg/BL_glctx.h"
#include    "base_l/BL_base.h"
#include    <errno.h>
#include    <stdio.h>
#include    <string.h>
#include    <malloc.h>

static BL_glctxcallbacks_t gc_defaultcallbacks = BL_GLCTXCALLBACK_DEF;

static BL_glctx_t g_glctx = {
    NULL, 0, 
    { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None }, 
    NULL, 0, { 0 },
    BL_GLWINDOWS_NUM,
    { 0 }, // xev
    false, // b_ontinue
    {
        NULL
    }
};

int BL_GLCTX_donothing() { return 0; }

void BL_glctxcallbacks_copy(pBL_glctxcallbacks_t dst, pcBL_glctxcallbacks_t src)
{
    dst->drawscene = src->drawscene;
    dst->chgwindow = src->chgwindow;
    dst->movewindow = src->movewindow;
    dst->keypressed = src->keypressed;
    dst->keyreleased = src->keyreleased;
    dst->mousebuttonpressed = src->mousebuttonpressed;
    dst->mousebuttonreleased = src->mousebuttonreleased;
}

int BL_glctx_opendisplay()
{
    int err = ESUCCESS;
    do {
        if (g_glctx.dpy) break; // already opened.
        if ((g_glctx.dpy = XOpenDisplay(NULL)) == NULL) 
        {
            err = ENAVAIL;
            break;
        }
        g_glctx.root = DefaultRootWindow(g_glctx.dpy);
        g_glctx.vi = glXChooseVisual(g_glctx.dpy, 0, g_glctx.att);
        if (NULL == g_glctx.vi)
        {
            err = ENAVAIL;
            break;
        }
        printf("viaual %p selected\n", (void*)g_glctx.vi->visualid);
        g_glctx.swa.colormap =
        g_glctx.cmap = XCreateColormap(g_glctx.dpy, g_glctx.root, g_glctx.vi->visual, AllocNone);
        g_glctx.swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
            ButtonPressMask | ButtonReleaseMask;
        for (uint32_t i = 0; i < BL_GLWINDOWS_NUM; i++)
        {
            g_glctx.win[i] = NULL;
        }
    } while (0);
    return err;
}

int BL_glctx_openwindow(uint32_t i_window, const char* name, const BL_2i32_t win_pos, const BL_2u32_t win_size)
{
    int err = ESUCCESS;
    do {
        size_t cb_alloc = sizeof(BL_glwininfo_t) + strlen(name) + 1;
        pBL_glwininfo_t win = g_glctx.win[i_window] = (pBL_glwininfo_t)malloc(cb_alloc);
        strcpy(win->wi.name, name);
        BL_copy2(win->wi.position, win_pos);
        BL_copy2(win->wi.size, win_size);
        BL_glctxcallbacks_copy(&(win->callbacks), &gc_defaultcallbacks);
        win->win = XCreateWindow(g_glctx.dpy, g_glctx.root,
            win_pos[0], win_pos[1], win_size[0], win_size[1], 0,
            g_glctx.vi->depth, InputOutput, g_glctx.vi->visual, CWColormap | CWEventMask, &g_glctx.swa
        );
        XMapWindow(g_glctx.dpy, win->win);
        XStoreName(g_glctx.dpy, win->win, name);
        win->glc = glXCreateContext(g_glctx.dpy, g_glctx.vi, NULL, GL_TRUE);
        pBL_glwininfo_t win_dummy;
        err = BL_glctx_select(i_window, &win_dummy);
    } while (0);
    return err;
}

int BL_glctx_closewindow(uint32_t i_window)
{
    int err = ESUCCESS;
    do {
        if (NULL == g_glctx.win[i_window]) break;
        glXDestroyContext(g_glctx.dpy, g_glctx.win[i_window]->glc);
        XDestroyWindow(g_glctx.dpy, g_glctx.win[i_window]->win);
        free((void*)g_glctx.win[i_window]);
        g_glctx.win[i_window] = NULL;
    } while (0);
    return err;
}
int BL_glctx_new(uint32_t i_window, const char* name, const BL_2i32_t win_pos, const BL_2u32_t win_size)
{
    int err = ESUCCESS;
    do {
        // Open display if not opened.
        if (ESUCCESS != (err = BL_glctx_opendisplay()))
        {
            break;
        }
        // Close the target window if it has been already opened.
        if (ESUCCESS != (err = BL_glctx_closewindow(i_window)))
        {
            break;
        }
        // Open the target window.
        err = BL_glctx_openwindow(i_window, name, win_pos, win_size);
    } while (0);
    return err;
}

uint32_t BL_glctx_countwindow()
{
    uint32_t n = 0;
    for (uint32_t i = 0; i < BL_GLWINDOWS_NUM; i++)
    {
        if (g_glctx.win[i])
        {
            n++;
        }
    }
    return n;
}

int BL_glctx_delete(uint32_t i_window)
{
    int err = ESUCCESS;
    do {
        // close the window chosen by i_window.
        if (ESUCCESS != (err = BL_glctx_closewindow(i_window)))
        {
            break;
        }

        // close display if no window exist.
        if (BL_glctx_countwindow() == 0)
        {
            XCloseDisplay(g_glctx.dpy);
            g_glctx.dpy = NULL;
        }
    } while (0);
    return err;
}

int BL_glctx_select(uint32_t i_window, pBL_glwininfo_t *ppwin)
{
    int err = ESUCCESS;
    do {
        if (g_glctx.win[i_window] == NULL)
        {
            err = EINVAL;
            *ppwin = NULL;
            break;
        }
        *ppwin = g_glctx.win[i_window];
        glXMakeCurrent(g_glctx.dpy, (*ppwin)->win, (*ppwin)->glc);
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        g_glctx.i_current = i_window;
    } while (0);
    return err;
}

int BL_glctx_setcallbacks(uint32_t i_window, pcBL_glctxcallbacks_t callbacks)
{
    int err =ESUCCESS;
    do {
        if (g_glctx.win[i_window] == NULL)
        {
            err = EINVAL;
            break;
        }
        BL_glctxcallbacks_copy(&g_glctx.win[i_window]->callbacks, callbacks);
    } while (0);
    return err;
}

int BL_glctx_getcallbacks(uint32_t i_window, pBL_glctxcallbacks_t callbacks)
{
    int err = ESUCCESS;
    do {
        if (g_glctx.win[i_window] == NULL)
        {
            err = EINVAL;
            break;
        }
        BL_glctxcallbacks_copy(callbacks, &g_glctx.win[i_window]->callbacks);
    } while (0);
    return err;
}

uint32_t BL_glctx_findwindow(Window win)
{
    uint32_t index = 0;
    for (; index != BL_GLWINDOWS_NUM; index++)
    {
        if (win == g_glctx.win[index]->win)
        {
            break;
        }
    }
    return index;
}