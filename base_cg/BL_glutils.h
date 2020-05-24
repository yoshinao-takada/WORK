#ifndef BL_GLUTILS_H_
#define BL_GLUTILS_H_
#include "base_cg/BL_glctx.h"
#include "base_cg/BL_oomesh.h"
#include "base_l/BL_data2D.h"

#ifdef __cplusplus
extern "C" {
#endif
    void    BL_glutils_cleardrawing();
    void    BL_glutils_flushdrawing();
    void    BL_glutils_getandflushdrawing(pBL_data2D_t *pptex);

    /*!
    \brief set and bind a triangle array and a texture
    \param mesh [in] 3D point coordinates, texture coordinates, triangle vertex.
    disabled points in the mesh must be removed before calling this method.
    \param tex [in] texture information
    */
    int BL_glutils_settexturedmesh(pcBL_oomesh_t mesh, pcBL_data2D_t tex);

    /*!
    \brief simplest drawscene function. It is defined just as  a sample.
    */
    int BL_gutils_drawscene_simplest();

    /*!
    \brief not using shader, draw textured triangle array set by BL_glutils_settexturedmesh().
    */
    int BL_glutils_drawscene_texturedmesh();

    /*!
    \brief update window position and size in the context
    */
    int BL_glutils_update_windowgeometry();

    int BL_glutils_set_windowgeometry(uint32_t i_window, const BL_2i32_t pos, const BL_2u32_t size);

    /*!
    \brief keypressed event handler, simultaneously showing mouse (x,y) position.
    */
    int BL_glutils_showkeycode();

    /*!
    \brief mousebuttonpressed event handler, showing pressed mouse button and (x,y).
    */
    int BL_glutils_showmousebuttonandcoord();

    /*!
    \brief execute event handler tree
    \return 0: normal success, -1: normal event loop termination, others: unix errno
    */
    int BL_glutils_eventproc();
#ifdef __cplusplus
}
#endif
#endif