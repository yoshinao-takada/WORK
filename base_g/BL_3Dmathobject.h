#ifndef BL_3DMATHOBJECT_H_
#define BL_3DMATHOBJECT_H_
#include    "base_l/BL_types.h"
#include    ""
#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    BL_4u8_t bgra[8]; // [0,1]: 1st quadrant, [2,3], [4,5], [6,7]: 2nd-4th quadrant colors
    BL_2r32_t grid_pitch, grid_center, quadrant_center;
} BL_grid_pattern_t, *pBL_grid_pattern_t;
typedef const BL_grid_pattern_t *pcBL_grid_pattern_t;

typedef struct {
    float radius;
} BL_circdot_unit_pattern_t, *pBL_circdot_unit_pattern_t;
typedef const BL_circdot_unit_pattern_t *pcBL_circdot_unit_pattern_t;

typedef struct {
    float width;
} BL_linegrid_unit_pattern_t, *pBL_linegrid_unit_pattern_t;
typedef cosnt BL_linegrid_unit_pattern_t *pcBL_linegrid_unit_pattern_t;

typedef struct {
    BL_grid_pattern_t grid;
    BL_circdot_unit_pattern_t unit;
} BL_circdot_grid_pattern_t, *pBL_circdot_grid_pattern_t;
typedef const BL_circdot_grid_pattern_t *pcBL_circdot_grid_pattern_t;

typedef struct {
    BL_grid_pattern_t grid;
    BL_linegrid_unit_pattern_t unit;
} BL_line_grid_pattern_t, *pBL_line_grid_pattern_t;
typedef const BL_line_grid_pattern_t *pcBL_line_grid_pattern_t;

typedef struct {
    BL_grid_pattern_t grid;
} BL_checker_grid_pattern_t, *pBL_checker_grid_pattern_t;
typedef const BL_checker_grid_pattern_t *pcBL_checker_grid_pattern_t;

/*!
\brief function template type for color pattern generation
\param uv [in] 2D coordinate
\param params [in] BL_circdot_grid_pattern_t, BL_line_grid_pattern_t, or BL_checker_grid_pattern_t.
\return BGRA color value, alpha=0 ==> no object, 1..255: objects exist
*/
typedef BL_4u8_t (*BL_PATTERN_GEN)(const BL_2r32_t uv, const void* params);

/*!
\brief checker pattern with color change in quadrants
*/
BL_4u8_t    BL_PATTERN_GEN_checker(const BL_2r32_t uv, const void* params);

/*!
\brief circular dots pattern with color change in quadrants
*/
BL_4u8_t    BL_PATTERN_GEN_circdots(const BL_2r32_t uv, const void* params);

/*!
\brief line grid pattern with color change in quadrants
*/
BL_4u8_t    BL_PATTERN_GEN_line(const BL_2r32_t uv, const void* params);

typedef enum {
    BL_color_pattern_checker,
    BL_color_pattern_circdots,
    BL_color_pattern_line,
} BL_color_pattern_type_t;

define BL_COLOR_PATTERN_GENERATORS { \
    BL_PATTERN_GEN_checker, \
    BL_PATTERN_GEN_circdots, \
    BL_PATTERN_GEN_line }

typedef struct {
    BL_1r32_t   to_canonical_plane[16]; // coordinate translator to canonical plane
    BL_1r32_t   from_canonical_plane[16]; //coordinate translator from canonical plane
    BL_4r32_t   outline_rect; // [0,1]: rectangle origine, [2,3]: rectangle diagonal corner
    BL_PATTERN_GEN color_pattern_generator;
    union {
        BL_checker_grid_pattern_t checker;
        BL_circdot_grid_pattern_t circdot;
        BL_line_grid_pattern_t line;
    } color_pattern_parameters;
} BL_plate_t, *pBL_plate_t;
typedef const BL_plate_t *pcBL_plate_t;

typedef enum {
    BL_intersection_cross,
    BL_intersection_tangential,
    BL_intersection_none,
} BL_intersection_type_t;

/*!
*/
typedef BL_intersection_type_t (*BL_LINE_INTERSECTION)(const BL_3r32_t line0, const BL_3r32_t line_dir, const void* shape_conf);
#ifdef __cplusplus
}
#endif
#endif