#ifndef BL_BILINEARINTERP_H_
#define BL_BILINEARINTERP_H_
/*!
\file BL_bilinearinterp.h
\brief Bilinear interpolator object and related methods
*/
#include    "base_l/BL_containers.h"
#ifdef __cplusplus
extern "C" {
#endif
/*!
\struct BL_bilinearinterp_t
\brief  It encapsulates a bilinear interpolation coefficient table and base function values
on discritized points. a two variable scalar function f(x,y) is given on discrete points (Xi,Yj)
(i = 0..Nw-1, j = 0..Nh-1, where Nw:table width, Nh:table height).
(x,y) is defined in a world space. table origin node is assumed to be at (table_origin[0], table_origin[1])
in the world space.
*/
// Cell structure
//  ND0-----ND1     ND0: cell origin   
//  |       |       (ND1 - ND0).x : grid_pitch[0]
//  |       |       (ND2 - ND0).y : grid_pitch[1]
//  ND2-----ND3
//
// 2D table plane structure
//  ND(0,0)-----ND(Nw-1,0)
//  |           |
//  |           |
//  ND(0,Nh-1)--ND(Nw-1,Nh-1)
//  ND(0,0) locate at table_org_offset in the world coordinate space
//  dims of values = { Nw, Nh, 1,1,1,1,1,1 }, element type = double
//  dims of coeff = { Nw-1, Nh-1, 1,1,1,1,1,1 }, element type = double

    typedef struct {
        // offset of (table_origin - world_origin)
        BL_2r32_t table_org_offset;
        // world coordinate distance between two adjacent nodes in the table
        BL_2r32_t grid_pitch;
        // function values on each nodes
        pBL_arrayMD_t values;
        // bilinear coefficient of cells
        pBL_arrayMD_t coeff;
    } BL_bilinearinterp_t, *pBL_bilinearinterp_t;
    typedef const BL_bilinearinterp_t *pcBL_bilinearinterp_t;

    /*!
    \brief create a new bilinear interp table object
    \param wh_nodes [in] node count in width and height directions
    \param table_origin [in] coordinate of the table origin node in world coordinate space
    \param grid_pitch [in] horizontal and vertical distances between two adjacent nodes.
    */
    pBL_bilinearinterp_t BL_bilinearinterp_new(
        const BL_2u32_t wh_nodes,
        const BL_2r32_t table_origin,
        const BL_2r32_t grid_pitch);
    
    /*!
    \brief delete an existing object and set the pointer NULL, preventing from deleting
        not-initialized or already-deleted objects.
    \param ppobj [in,out] pointer-pointer to an object
    */
    void BL_bilinearinterp_delete(pBL_bilinearinterp_t* ppobj);

    /*!
    \brief fill coeff table
    \param obj [in,out]
    */
    void BL_bilinearinterp_fill_coeff(pBL_bilinearinterp_t obj);

    /*!
    \brief execute bilinear interpolation to get an approximaged value of function represented by values
    \param obj [in] interpolating object
    \param xy [in]
    \param value [out]
    \return unix errno compatible error number
    */
    int BL_bilinearinterp_calc_value(pcBL_bilinearinterp_t obj, const BL_2r32_t xy, BL_1r32_t* value);

    /*!
    \brief execute bilinear interpolation on uniformly aligned 2D grid
    \param obj [in] interpolating object
    \param begin [in] begining of the grid
    \param pitch [in] grid pitch of the grid
    \param wh [in] width and height of grid count
    \param ppvalues [out] values on the grid nodes
    \return unix errno compatible error number
    */
    int BL_bilinearinterp_calc_values(
        pcBL_bilinearinterp_t obj,
        const BL_2r32_t begin,
        const BL_2r32_t pitch,
        pBL_arrayMD_t values);

    /*!
    \brief get a range of (x,y) applicable to interpolation
    (bebin[0] <= x < end[0], begin[1] <= y < end[1])
    \param obj [in] interpolating object
    \param begin [out] beginning of the range
    \param end [out] end of the range
    */
    void    BL_bilinearinterp_xy_range(pcBL_bilinearinterp_t obj, BL_2r32_t begin, BL_2r32_t end);

    /*!
    \brief create a new bilinear interp table object
    \param wh_nodes [in] node count in width and height directions
    \param table_origin [in] coordinate of the table origin node in world coordinate space
    \param grid_pitch [in] horizontal and vertical distances between two adjacent nodes.
    \param dim [in] dimension of each value
    */
    pBL_bilinearinterp_t BL_bilinearinterpv_new(
        const BL_2u32_t wh_nodes,
        const BL_2r32_t table_origin,
        const BL_2r32_t grid_pitch,
        uint32_t dim_element_vector);

    
    /*!
    \brief delete an existing object and set the pointer NULL, preventing from deleting
        not-initialized or already-deleted objects.
    \param ppobj [in,out] pointer-pointer to an object
    */
   #define  BL_bilinearinterpv_delete(ppobj)    BL_bilinearinterp_delete(ppobj)


    /*!
    \brief fill coeff table
    \param obj [in,out]
    */
    void BL_bilinearinterpv_fill_coeff(pBL_bilinearinterp_t obj);


    /*!
    \brief execute bilinear interpolation to get an approximaged value of function represented by values
    \param obj [in] interpolating object
    \param xy [in]
    \param value [out]
    \return unix errno compatible error number
    */
    int BL_bilinearinterpv_calc_value(pcBL_bilinearinterp_t obj, const BL_2r32_t xy, BL_1r32_t* value);

    /*!
    \brief get a range of (x,y) applicable to interpolation
    (bebin[0] <= x < end[0], begin[1] <= y < end[1])
    \param obj [in] interpolating object
    \param begin [out] beginning of the range
    \param end [out] end of the range
    */
    void    BL_bilinearinterpv_xy_range(pcBL_bilinearinterp_t obj, BL_2r32_t begin, BL_2r32_t end);

    /*!
    \brief execute bilinear interpolation on uniformly aligned 2D grid
    \param obj [in] interpolating object
    \param begin [in] begining of the grid
    \param pitch [in] grid pitch of the grid
    \param wh [in] width and height of grid count
    \param ppvalues [out] values on the grid nodes
    \return unix errno compatible error number
    */
    int BL_bilinearinterpv_calc_values(
        pcBL_bilinearinterp_t obj,
        const BL_2r32_t begin,
        const BL_2r32_t pitch,
        const BL_2u32_t wh,
        pBL_arrayMD_t* ppvalues);

    /*!
    \brief create an interpolator object from B8G8R8 color pixel array
    \param wh_nodes [in] 2D array size of bgr_pixels
    \param bgr_pixels [in] pointer to the 1st element of the color pixel array
    \return interpolator object 
    */
    pBL_bilinearinterp_t BL_bilinearinterpv_newBGR(
        const BL_2u32_t wh_nodes,
        const BL_3u8_t* bgr_pixels
    );

    /*!
    \brief get interpolated image as B8G8R8 color image
    \param obj [in] interpolating object
    \param begin [in] begining of the grid
    \param pitch [in] grid pitch of the grid
    \param wh [in] width and height of grid count
    \param ppimage [out] BGR image created by interpolation
    \return unix errno compatible error number
    */
    int BL_bilinearinterpv_getBGR(
        pcBL_bilinearinterp_t obj,
        const BL_2r32_t begin,
        const BL_2r32_t pitch,
        const BL_2u32_t wh,
        pBL_arrayMD_t* ppimage);
#ifdef __cplusplus
}
#endif
#endif
