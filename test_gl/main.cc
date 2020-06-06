#include    "test_gl/BL_glctx.h"
#include    "test_gl/BL_glxcommon.h"
#include    "base_l/BL_base.h"
#include    "base_l/BL_futils.h"
#include    "base_l/BL_containers.h"
#include    <opencv2/core.hpp>
#include    <opencv2/imgproc.hpp>
#include    <opencv2/imgcodecs.hpp>
#include    <stdio.h>
#define     WIDTH   640
#define     HEIGHT  480
#define     WH      {WIDTH,HEIGHT};
#define     APPNAME "gltest_ctx"

int main(int ac, const char* av[])
{
    int err = ESUCCESS;
    const BL_2u32_t wh = WH;
    do {
        if (ESUCCESS != (err = BL_glxcommon_init()))
        {
            break;
        }
        pBL_glctx_t ctx = BL_glctx_new(wh, APPNAME);
        if (ESUCCESS != (err = BL_glctx_clear(ctx)))
        {
            break;
        }
        BL_glctx_delete(&ctx);
        BL_glxcommon_cleanup();
    } while (false);
    return err;
}