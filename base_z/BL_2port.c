#include    "base_z/BL_2port.h"
#include    "base_l/BL_errno.h"

static int lossy_LC_series_shunt_z(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m)
{
    const BL_1c128_t zb = CMPLX(section->R, w * section->L - 1.0/(w * section->C));
    const BL_1c128_t Rs_complex = CMPLX(section->Rs, 0.0);
    m[0] = m[3] = Rs_complex + zb;
    m[1] = m[2] = zb;
    return ESUCCESS;
}

static int lossy_LC_parallel_series_y(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m)
{
    const BL_1c128_t ybc = CMPLX(0.0, w * section->C);
    const BL_1c128_t ybl = CMPLX(1.0,0.0)/CMPLX(section->R, section->L * w);
    const BL_1c128_t yb = ybc + ybl;
    const BL_1c128_t Gs_complex = CMPLX(section->Gs, 0.0);
    m[0] = m[3] = Gs_complex + yb;
    m[1] = m[2] = -yb;
    return ESUCCESS;
}

int BL_2port_LC_section_z(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m)
{
    int err = ESUCCESS;
    do {
        switch (section->topology)
        {
            case BL_2port_topology_shunt_2port:
            {
                switch (section->element_type)
                {
                    case BL_linear_element_lossy_LC_series:
                    {
                        err = lossy_LC_series_shunt_z(section, w, m);
                        break;
                    }
                    default:
                    {
                        err = EINVAL;
                        break;
                    }                   
                }
                break;
            }
            case BL_2port_topology_series_2port:
            {
                err = EINVAL;
                break;
            }
            case BL_2port_topology_series_shunt:
            case BL_2port_topology_shunt_series:
            default:
            {
                err = EINVAL;
                break;
            }
        }
    } while (0);
    return err;
}

int BL_2port_LC_section_y(pcBL_2port_LC_section_t section, double w, BL_1c128_t* m)
{
    int err = ESUCCESS;
    do {
        switch (section->topology)
        {
            case BL_2port_topology_shunt_2port:
            {
                err = EINVAL;
                break;
            }
            case BL_2port_topology_series_2port:
            {
                switch (section->element_type)
                {
                    case BL_linear_element_lossy_LC_parallel:
                    {
                        err = lossy_LC_parallel_series_y(section, w, m);
                        break;
                    }
                    default:
                    {
                        err = EINVAL;
                        break;
                    }
                }
                break;
            }
            case BL_2port_topology_series_shunt:
            case BL_2port_topology_shunt_series:
            default:
            {
                err = EINVAL;
                break;
            }
        }
    } while (0);
    return err;
}