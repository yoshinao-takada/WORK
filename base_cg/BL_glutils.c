#include    "base_cg/BL_glutils.h"
#include    <stdio.h>
extern BL_glctx_t g_glctx;

void BL_glutils_cleardrawing()
{
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void BL_glutils_flushdrawing()
{
    if (g_glctx.win[g_glctx.i_current])
    {
        glXSwapBuffers(g_glctx.dpy, g_glctx.win[g_glctx.i_current]->win);
    }
}

void    BL_glutils_getandflushdrawing(pBL_data2D_t *pptex)
{
    do {
        if (*pptex)
        {
            free((void*)(*pptex));
        }
        pBL_glwininfo_t wininfo = g_glctx.win[g_glctx.i_current];
        if (wininfo == NULL)
        {
            break;
        }
        // init texture buffer
        *pptex = BL_data2D_alloc(wininfo->wi.size, sizeof(BL_3u8_t));
        glCopyPixels(0,0,(*pptex)->wh[0], (*pptex)->wh[1], GL_COLOR);
        glReadPixels(0,0,(*pptex)->wh[0], (*pptex)->wh[1], GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)((*pptex)+1));
        BL_glutils_flushdrawing();
    } while (0);
}

int BL_glutils_settexturedmesh(pcBL_oomesh_t mesh, pcBL_data2D_t tex)
{
    int err = ESUCCESS;
    do {
        // bind texture
        glGenTextures(1, &(g_glctx.win[g_glctx.i_current]->gl_texture));
        glBindTexture(GL_TEXTURE_2D, g_glctx.win[g_glctx.i_current]->gl_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->wh[0], tex->wh[1], 0, GL_BGR, GL_UNSIGNED_BYTE, tex->ptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // Specify how texture values combine with current surface color values.
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        // Turn on OpenGL texturing.
        glEnable(GL_TEXTURE_2D);

        // register vertex and txture coordinates
        glVertexPointer(3, GL_FLOAT, 0, mesh->vert);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh->tex);
    } while (0);
    return err;
}

int BL_glutils_drawscene_texturedmesh()
{
    int err = ESUCCESS;
    do {
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.75f, -0.75f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(+0.75f, +0.75f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
    } while (0);
    return err;
}

int BL_glutils_drawscene_simplest()
{
    int err = ESUCCESS;
    do {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 20.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(-0.75f, -0.75f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(+0.75f, +0.75f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
        glEnd();
    } while (0);
    return err;
}

int BL_glutuils_update_windowgeometry()
{
    int err = ESUCCESS;
    do {
        pBL_glwininfo_t wininfo = g_glctx.win[g_glctx.i_current];
        if (wininfo == NULL) break;
        BL_2i32_t xy;
        BL_2u32_t wh;
        uint32_t border, depth;
        Status s = XGetGeometry(g_glctx.dpy, wininfo->win, &g_glctx.root, xy, xy+1, wh, wh+1,
            &border, &depth);
        if (s == 0)
        {
            err = ENODATA;
            break;
        }
        BL_copy2(wininfo->wi.position, xy);
        BL_copy2(wininfo->wi.size, wh);
    } while (0);
    return err;
}

int BL_glutils_set_windowgeometry(uint32_t i_window, const BL_2i32_t pos, const BL_2u32_t size)
{
    int err = ESUCCESS;
    do {
        pBL_glwininfo_t wininfo = g_glctx.win[g_glctx.i_current];
        if (wininfo == NULL)
        {
            err = ENODATA;
            break;
        }

        unsigned int which_to_change = CWX | CWY | CWWidth | CWHeight;
        XWindowChanges chg = {
            pos[0], pos[1], size[0], size[1], 0, 0, 0
        };
        Status s = XConfigureWindow(g_glctx.dpy, wininfo->win, which_to_change, &chg);
        if (s == 0)
        {
            err = EINVAL;
            break;
        }
        err= BL_glutils_update_windowgeometry();
    } while (0);
}

int BL_glutils_showkeycode()
{
    int err = ESUCCESS;
    do {
        printf("key event\n");
        XKeyEvent* ev = &g_glctx.xev.xkey;
        printf("(x,y)=(%d,%d), keycode = 0x%04x(%d), state = 0x%04x(%d)\n",
            ev->x, ev->y, ev->keycode, ev->keycode, ev->state, ev->state);
    } while (0);
    return err;
}

int BL_glutils_showmousebuttonandcoord()
{
    int err = ESUCCESS;
    do {
        printf("mouse event\n");
        XButtonEvent* ev = &g_glctx.xev.xbutton;
        printf("(x,y)=(%d,%d), button = 0x%04x(%d), state = 0x%04x(%d)\n",
            ev->x, ev->y, ev->button, ev->button, ev->state, ev->state);
    } while (0);
    return err;
}

int BL_glutils_eventproc()
{
    int err = ESUCCESS;
    do {
        XNextEvent(g_glctx.dpy, &g_glctx.xev);
        uint32_t i_window = BL_glctx_findwindow(g_glctx.xev.xany.window);
        BL_glwininfo_t * wininfo = NULL;
        if (i_window == BL_GLWINDOWS_NUM)
        {
            break;
        }
        else
        {
            BL_glctx_select(i_window, &wininfo);
        }        
        switch (g_glctx.xev.type)
        {
            case Expose:
            {
                if (ESUCCESS != (err = BL_glutils_update_windowgeometry())) break;
                if (ESUCCESS != (err = BL_glutils_drawscene_simplest())) break;
                break;
            }
            case KeyPress:
            case KeyRelease:
            {
                if (ESUCCESS != (err = BL_glutils_showkeycode())) break;
                break;
            }
            case ButtonPress:
            case ButtonReleaseMask:
            {
                if (ESUCCESS != (err = BL_glutils_showmousebuttonandcoord())) break;
                break;
            }
            default:
            {
                break;
            }
        }
    } while (0);
    return err;
}