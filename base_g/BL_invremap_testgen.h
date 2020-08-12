// This file is included only by BL_invremap.h.

// test pattern generator conf
typedef struct {
    BL_4u32_t roi;
    BL_2r32_t shift0; // value at the origin of roi
    BL_2r32_t dshiftx; // delta of shift value variation between adjacent two nodes in h-direction.
    BL_2r32_t dshifty; // delta of shift value variation between adjacent two nodes in v-direction.
} BL_source_shift_map_TPG_conf_t, *pBL_source_shift_map_TPG_conf_t;
typedef const BL_source_shift_map_TPG_conf_t *pcBL_source_shift_map_TPG_conf_t;

/*!
\brief create a new object which can be destroy by BL_SAFEFREE().
*/
int BL_source_shift_map_new(const BL_2u32_t work_size, pBL_source_shift_map_t* ppmap);

/*!
\brief create a test pattern in the map
\param map [in,out] 
\param conf [in] configuration info of the test pattern
*/
int BL_source_shift_map_create(pBL_source_shift_map_t map, pcBL_source_shift_map_TPG_conf_t conf);

/*!
\brief save a source shift map in a csv file
\param map [in] source shift map
\param pf [in,out] text file stream of the csv file
\return errno compatible number
*/
int BL_source_shift_map_savecsv(pcBL_source_shift_map_t map, FILE* pf);