#ifndef BL_2PORT_H_
#define BL_2PORT_H_
#include    <base_l/BL_base.h>
#include    <base_l/BL_types.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    BL_linear_element_lossy_LC_series,
    BL_linear_element_lossy_LC_parallel,
} BL_linear_element_type_t;

typedef enum {
    BL_2port_topology_shunt_2port,
    BL_2port_topology_series_2port,
    BL_2port_topology_series_shunt, // 1st stage: series branch, 2nd stage: shunt branch
    BL_2port_topology_shunt_series, // 1st stage: shunt branch , 2nd stage: series branch
} BL_2port_topology_t;

typedef struct {
    BL_linear_element_type_t element_type;
    BL_2port_topology_t topology;
    double L, C, R;
    double Gs, Rs; // shunt/series protection
} BL_2port_LC_section_t, *pBL_2port_LC_section_t;
typedef const BL_2port_LC_section_t *pcBL_2port_LC_section_t;

/*!
\brief impedance matrix of the 2-port section at an angular frequency w
\param section [in] 2-port section definition
\param w [in] angular frequency
\param m [out] pointer to the matrix 1st element
\return ESUCCESS if the section is T section, otherwise EINVAL
*/
int BL_2port_LC_section_z(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m);


/*!
\brief admittance matrix of the 2-port section at an angular frequency w
\param section [in] 2-port section definition
\param w [in] angular frequency
\param m [out] pointer to the matrix 1st element
\return ESUCCESS if the section is PI section, otherwise EINVAL
*/
int BL_2port_LC_section_y(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m);
#ifdef __cplusplus
}
#endif
#endif /* BL_2PORT_H_ */