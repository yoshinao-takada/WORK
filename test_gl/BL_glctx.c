#include    "test_gl/BL_glctx.h"
#include    "test_gl/BL_glxctx.h"

pBL_glctx_t BL_glctx_new(const BL_2u32_t wh, const char* name)
{
    pBL_glxctx_t p = BL_glxctx_new(wh, name);
    return &(p->base);
}

void    BL_glctx_delete(pBL_glctx_t* ppobj)
{
    BL_glxctx_delete((pBL_glxctx_t*)ppobj);
}

int     BL_glctx_init(pBL_glctx_t ctx, const void* params)
{
    return BL_glxctx_init((pBL_glxctx_t)ctx, params);
}

int     BL_glctx_clear(pBL_glctx_t ctx)
{
    return BL_glxctx_clear((pBL_glxctx_t)ctx);
}

int     BL_glctx_draw(pBL_glctx_t ctx, BL_glctx_drawfunc draw_func, const void* obj3D)
{
    return BL_glxctx_draw((pBL_glxctx_t)ctx, draw_func, obj3D);
}

int     BL_glctx_flush(pBL_glctx_t ctx, pBL_arrayMD_t* ppimage)
{
    return BL_glxctx_flush((pBL_glxctx_t)ctx, ppimage);
}