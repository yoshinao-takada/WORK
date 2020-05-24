#ifndef BL_GLCTX_H_
#define BL_GLCTX_H_
#include <base_l/BL_base.h>
#include <base_l/BL_types.h>

#ifdef __cplusplus
extenr "C" {
#endif

typedef struct {
    BL_2i32_t xy;
    BL_2u32_t wh;
    BL_4r32_t clear_color;
    char* window_caption;
    double  *mat_modelview, *mat_projection, *mat_viewport;
} BL_glctx_t, *pBL_glctx_t;
typedef const BL_glctx_t *pcBL_glctx_t;

/*!
\brief create a new object but NOT bind a new window yet.
*/
pBL_glctx_t BL_glctx_new(const BL_2i32_t xy, const BL_2u32_t wh, const BL_4r32_t clear_color, const char* window_caption);

/*!
\brief set model-view matrix with parameters same as gluLookAt()
*/
void BL_glctx_init_modelviewmatrix(pBL_glctx_t ctx, const BL_3r64_t eye, const BL_3r64_t lookat, const BL_3r64_t up);

/*!
\brief set projection matrix with parameters same as gluPerspective
*/
void BL_glctx_init_projectionmatrix(pBL_glctx_t ctx, double fovy, BL_2r64_t zrange);

/*!
\brief bind a physical window served by X-Window, Windows, etc.
*/
void BL_glctx_bind_window(pBL_glctx_t ctx);

/*!
\brief update geometrical parameters with the current status of the bound window.
*/
void BL_glctx_update_geometry(pBL_glctx_t ctx);

/*!
\brief clear the current swap buffer to render a new image.
*/
void BL_glctx_clear_image(pBL_glctx_t ctx);

typedef int (*BL_GLCTX_RENDERCALLBACK)(const void* data);

/*!
\brief render a new image
\param ctx [in,out] rendering context
\param render_callback [in] rendering function
\param data [in] rendering data
\return render_callback return code
*/
int BL_glctx_render(pBL_glctx_t ctx, BL_GLCTX_RENDERCALLBACK render_callback, const void* data);

/*!
\brief retrieve the latest image and swap buffer.
\param ctx [in,out] rendering context
\param wh [out] image size
\param pixbuf [out] B8G8R8 buffer in row major layout; if wh == NULL || pixbuf == NULL, don't retrieve the image.
*/
int BL_glctx_flushout(pBL_glctx_t ctx, BL_2u32_t wh, void** pixbuf);
#ifdef __cplusplus
}
#endif
#endif