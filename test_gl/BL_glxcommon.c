#include    "test_gl/BL_glxcommon.h"
#include    <stdbool.h>
#include    <string.h>
#include    <stdio.h>
#include    <errno.h>
#include    <stdlib.h>

static BL_glxcommon_t common_context = {0};

static const int visual_attribs[] = VISUAL_ATTRIB_DEFS;

static bool isExtensionSupported(const char* extList, const char* extension)
{
    const char* start;
    const char  *where, *terminator;

    // extension names do not have spaces and is not an empty string
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
    {
        return false;
    }

    // parsing extension string
    for (start = extList;;)
    {
        where = strstr(start, extension);
        if (!where) break; // extension string was NOT found in extList
        terminator = where + strlen(extension);
        if (where == start || *(where - 1) == ' ')
        {
            if (*terminator == ' ' || *terminator == '\0')
            {
                return true;
            }
        }
        start = terminator;
    }
    return false;
}


static bool ctxErrorOccurred = false;
static int ctxErrorHandler( Display *dpy, XErrorEvent *ev )
{
    ctxErrorOccurred = true;
    return 0;
}

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
int BL_glxcommon_init()
{
    int err = ESUCCESS;
    Display* display = NULL;
    do {
        if (NULL == (common_context.display = XOpenDisplay(NULL)))
        {
            err = ENODEV;
            break;
        }
        display = common_context.display;

        // GLX version must be >= 1.3
        if (!glXQueryVersion(display, &common_context.glx_version[0], &common_context.glx_version[1]) ||
            (common_context.glx_version[0]==1 && common_context.glx_version[1] < 3) ||
            (common_context.glx_version[0] < 1))
        {
            err = EINVAL;
            break;
        }
        
        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
        if (!fbc)
        {
            err = EINVAL;
            break;
        }
        printf("Found %d matching FB configs.\n", fbcount);

        // get the best fbc
        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
        for (int i = 0; i != fbcount; i++)
        {
            XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc[i]);
            if (vi)
            {
                int samp_buf, samples;
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);
                printf("Matching fbconfig %d, visual ID 0x%02x: SAMPLE_BUFFERS = %d, SAMPLES = %d\n",
                    i, vi->visualid, samp_buf, samples);
                if (best_fbc < 0 || (samp_buf && (samples > best_num_samp)))
                {
                    best_fbc = i;
                    best_num_samp = samples;
                }
                if (worst_fbc < 0 || !samp_buf || (samples < worst_num_samp))
                {
                    worst_fbc = i;
                    worst_num_samp = samples;
                }
            }
            XFree(vi);
        }
        common_context.bestFbc = fbc[best_fbc];
        XFree(fbc);

        // Get a visual which is used by XCreateWindow().
        common_context.vi = glXGetVisualFromFBConfig(display, common_context.bestFbc);
        common_context.swa.colormap = common_context.cmap =
            XCreateColormap(display, RootWindow(display, common_context.vi->screen), common_context.vi->visual, AllocNone);
        common_context.swa.background_pixmap = None;
        common_context.swa.border_pixel = 0;
        common_context.swa.event_mask = StructureNotifyMask;

        glXCreateContextAttribsARBProc glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
            glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");
        common_context.ctx = 0;
        ctxErrorOccurred = false;
        int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);
        // check for the GLX_ARB_create_context extension string and the function.
        const char *glxExts = glXQueryExtensionsString( display,
                                                  DefaultScreen( display ) );
        if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB)
        {
            printf("glXCreateContextAttribsARB() not found ... using old-style GLX context\n");
            common_context.ctx = glXCreateNewContext(display, common_context.bestFbc, GLX_RGBA_TYPE, 0, True);
        }
        else
        {
            int context_attribs[] =
            {
                GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
                GLX_CONTEXT_MINOR_VERSION_ARB, 0,
                //GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                None
            };
            common_context.ctx = glXCreateContextAttribsARB(display, common_context.bestFbc, 0, True, context_attribs);
            XSync(display, False);
            if (!ctxErrorOccurred && common_context.ctx)
            {
                printf("Created GL 3.0 context\n");
            }
            else
            {
                context_attribs[1] = 1;
                context_attribs[3] = 0;
                ctxErrorOccurred = false;
                printf("Failed to create GL 3.0 context ... using old-style GLX context\n");
                common_context.ctx = glXCreateContextAttribsARB(display, common_context.bestFbc, 0, True, context_attribs);
            }
        }
        XSync(display, False);
        XSetErrorHandler(oldHandler);            
        if (ctxErrorOccurred || !common_context.ctx)
        {
            printf("Failed to create an OpenGL context\n");
            exit(1);
        }
        printf("%s GLX rendering context obtained\n", glXIsDirect(display, common_context.ctx) ? "Direct" : "Indirect");
    } while (false);
    return err;
}

pBL_glxcommon_t BL_glxcommon_get()
{
    return &common_context;
}

void BL_glxcommon_cleanup()
{
    XFree(common_context.vi);
    XCloseDisplay(common_context.display);
    common_context.display = NULL;
}