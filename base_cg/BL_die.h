#ifndef BL_DIE_H_
#define BL_DIE_H_
#include    <base_l/BL_data2D.h>
#include    <base_cg/BL_oomesh.h>
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct {
        uint32_t cube_edge_length;
        float    large_dot_R, small_dot_R;
    } BL_dieconf_t, *pBL_dieconf_t;

    typedef const BL_dieconf_t *pcBL_dieconf_t;

    int BL_die_tex(pcBL_dieconf_t conf, pBL_data2D_t* tex);

    int BL_die_mesh(pcBL_dieconf_t conf, pBL_oomesh_t* mesh);
#ifdef __cplusplus
}
#endif
#endif