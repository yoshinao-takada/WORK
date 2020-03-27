#ifndef BL_TRISTRIP_H_
#define BL_TRISTRIP_H_
#include    <base_l/BL_containers.h>
#include    <base_cg/BL_oomesh.h>
#ifdef __cplusplus
extern "C" {
#endif
    // linkable array of BL_6r32_t is BL_tristrip_t.
    // element[0..2] : vertex coordinate
    // element[3..4] : texture coordinate
    // element[5] : reserved, allways nanf
    typedef BL_linkable_array_t BL_tristripnode_t, *pBL_tristripnode_t;
    typedef const BL_tristripnode_t *pcBL_tristripnode_t;

    typedef enum {
        BL_tristrip_strip_notendofrow = 1,
        BL_tristrip_strip_withtriangles = 2,
    } BL_tristrip_strip_endstatus_t;
    /*!
    \brief retrieve a peice of triangle strip from mesh.
    \param mesh [in] source organized mesh
    triangles are laid out as folows.
    0---2---4---6
    | / | / | / | /
    1---3---5---7--
    \param at [in,out] in:position to start retrieving, out:position to end retrieving
    \param strip_anchor [out] the anchor of the triangle strip. strip_anchor is a pure anchor node
    without any data but only with link pointers.
    \return bitwise or of BL_tristrip_strip_endstatus_t
    */
    int BL_tristrip_strip(pcBL_oomesh_t mesh, BL_2u32_t at, pBL_linkable_t strip_anchor);

    /*
    trunk_anchor--tristrip0.trunk--tristrip1.trun--...
                       |                |
                       |           tristrip1.strip_anchor--tristripnode10--tristripnode11--...
                       |
                  tristrip0.strip_anchor--tristripnode00--tristripnode01--...
    */
    typedef struct {
        BL_linkable_t trunk; // trunk link
        BL_linkable_t strip_anchor; // anchor of triangle strip
    } BL_tristrip_t, *pBL_tristrip_t;

    typedef const BL_tristrip_t *pcBL_tristrip_t;

    pBL_tristrip_t BL_tristrip_new();
    /*!
    \brief retrieve multiple pieces of triangle strips from a mesh.
    \param mesh [in] source organized mesh
    \param trunk_anchor [out] retrieved list of triangle strip. trunk_anchor is a pure anchor node
    without any data but only with link pointers.
    */
    void BL_tristriplist_strip(pcBL_oomesh_t mesh, pBL_linkable_t trunk_anchor);
#ifdef __cplusplus
}
#endif
#endif