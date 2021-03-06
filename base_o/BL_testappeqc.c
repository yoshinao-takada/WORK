#include    "base_o/BL_testappeqc.h"
#include    "base_g/BL_matexport.h"
#define PRINT_ARGS(stream,vdim,v,ptr_to_f) \
        printf("%s,x[*]", __FUNCTION__); \
        for (uint32_t i = 0; i != vdim; i++)  printf(",%f", v[i]); \
        printf(",f,%f\n", *ptr_to_f);

static pcBL_matfnr32RM_t fnmat = NULL;
/*
Sample problem: Zmin on a cylinder intersecting a plane
Cylinder: BL_3r32_t axis0 (a point on the axis), BL_3r32_t axis_dir (the uint directioin vector of the axis)
    BL_1r32_t r0 (radius of the cylinder)
Plane: BL_3r32_t plane0 (a point on the plane), BL_3r32_t plane_normal (the unit normal vector of the plane)
*/

int TEST_objective(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    const BL_1r32_t *vn = (const BL_1r32_t*)params;
    BL_1r32_t f_;
    fnmat->mul(x, vn, &f_, 1, 3, 1);
    *f = f_;
    // PRINT_ARGS(stdout, vardim, x, f);
    return 0;
}

int TEST_eqc0(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    pcTEST_plane_t plane = (pcTEST_plane_t)params;
    const BL_1r32_t* P0 = plane->p0; // plane reference point
    const BL_1r32_t* Vn = plane->vn; // plane unit normal vector
    BL_3r32_t Vs, Vst, VnVsn;
    BL_1r32_t Vsn, VstVst;
    fnmat->sub(x,P0, Vs, 3, 1); // Vs = x - P0
    fnmat->mul(Vs, Vn, &Vsn, 1, 3, 1); // Vsn = <Vs, Vn>
    fnmat->muls(Vn, 3, 1, Vsn, VnVsn); // VnVsn = Vn * Vsn
    fnmat->sub(Vs, VnVsn, Vst, 3, 1); // Vst = Vs - VnVsn
    fnmat->mul(Vst,Vst,&VstVst, 1, 3, 1); // VstVst = <Vst, Vst>
    *f = Vsn/sqrtf(VstVst);
    // PRINT_ARGS(stdout, vardim, x, f);
    return 0;
}

int TEST_eqc1(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    pcTEST_cylinder_t cyl = (pcTEST_cylinder_t)params;
    BL_3r32_t PsP0, P1, Vr, PsP0Vd_Vd;
    BL_1r32_t PsP0Vd, VrVr;
    fnmat->sub(x, cyl->axis0, PsP0, 3, 1); // PsP0 = Ps - P0
    fnmat->mul(PsP0, cyl->axis_dir, &PsP0Vd, 1, 3, 1); // PsP0Vd = <Ps - P0, Vd>
    fnmat->muls(cyl->axis_dir, 3, 1, PsP0Vd, PsP0Vd_Vd); // PsP0Vd_Vd = <Ps - P0, Vd>Vd
    fnmat->add(cyl->axis0, PsP0Vd_Vd, P1, 3, 1); // P1 = P0 + <Ps - P0, Vd>Vd
    fnmat->sub(x, P1, Vr, 3, 1); // Vr = Ps - P1
    fnmat->mul(Vr, Vr, &VrVr, 1, 3, 1); // VrVr = <Vr,Vr> = |Vr|^2
    *f = VrVr - cyl->R * cyl->R;
    // PRINT_ARGS(stdout, vardim, x, f);
    return 0;
}

int TEST_lagrange(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    pcBL_constraint_t constraints = (pcBL_constraint_t)params;
    int err = ESUCCESS;
    do {
        // check vardim consistency
        if ((constraints->vardim_objective + constraints->eqc_dim + constraints->neqc_dim) != vardim)
        {
            err = EINVAL;
            break;
        }
        // calculate objective function
        if (ESUCCESS != (err = constraints->fn_objective(constraints->vardim_objective, x, constraints->params_objective, f)))
        {
            break;
        }
        // calculate equality constraints
        BL_1r32_t f_constraint;
        const BL_1r32_t* h = x + constraints->vardim_objective;
        BL_cptr_t i_eqc = BL_array_cbegin(constraints->eqc);
        for (uint32_t i = 0; i!=constraints->eqc_dim; i++)
        {
            BL_OBJECTIVE_FUNC fn_eqc = (BL_OBJECTIVE_FUNC)(*i_eqc._2uptr)[0];
            const void* params_eqc = (const void*)(*i_eqc._2uptr)[1];
            err = fn_eqc(constraints->vardim_objective, x, params_eqc, &f_constraint);
            *f += (h[i] * f_constraint);
            i_eqc._2uptr++;
        }
    } while (0);
    // PRINT_ARGS(stdout, vardim, x, f);
    return err;
}

int TEST_penalty(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
{
    pcBL_constraint_t constraints = (pcBL_constraint_t)params;
    int err = ESUCCESS;
    do {
        // calculate objective function
        float f_objective;
        if (ESUCCESS != (err = constraints->fn_objective(constraints->vardim_objective, x, constraints->params_objective, &f_objective)))
        {
            break;
        }
        *f = f_objective * f_objective;
        // calculate equality constraints
        BL_1r32_t f_constraint;
        BL_cptr_t i_eqc = BL_array_cbegin(constraints->eqc);
        for (uint32_t i = 0; i!=constraints->eqc_dim; i++)
        {
            BL_OBJECTIVE_FUNC fn_eqc = (BL_OBJECTIVE_FUNC)(*i_eqc._3uptr)[0];
            const void* params_eqc = (const void*)(*i_eqc._3uptr)[1];
            if (ESUCCESS != (err = fn_eqc(constraints->vardim_objective, x, params_eqc, &f_constraint))) 
            {
                break;
            }
            BL_1r32_t penalty_factor = (BL_1r32_t)(*i_eqc._3uptr)[2];
            *f += penalty_factor * f_constraint * f_constraint;
            i_eqc._3uptr++;
        }
    } while (0);
    // PRINT_ARGS(stdout, vardim, x, f);
    return err;
}
int TEST_create(pcTEST_plane_t plane, pcTEST_cylinder_t cylinder, pBL_cg_t* optimizer)
{
    static const BL_3r32_t Zn = {0.0f, 0.0f, 1.0f};
    static const BL_1r32_t XV0[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    int err = ESUCCESS;
    pBL_constraint_t constraints = NULL;
    if (!fnmat) { fnmat = BL_matfnr32RM(); }
    do {
        if (!optimizer || *optimizer)
        {
            err = EINVAL;
            break;
        }
        pBL_array_t eqc = BL_array_new(2, BL_3uptr_t);
        pBL_array_t neqc = BL_array_new(0, BL_3uptr_t);
        BL_ptr_t i_eqc = BL_array_begin(eqc);
        i_eqc._3uptr[0][0] = (BL_1uptr_t)TEST_eqc0;
        i_eqc._3uptr[0][1] = (BL_1uptr_t)plane;
        i_eqc._3uptr[0][2] = (BL_1uptr_t)1.0e4f;
        i_eqc._3uptr[1][0] = (BL_1uptr_t)TEST_eqc1;
        i_eqc._3uptr[1][1] = (BL_1uptr_t)cylinder;
        i_eqc._3uptr[1][2] = (BL_1uptr_t)1.0e4f;
        constraints = BL_constraint_new(eqc, neqc, 3, TEST_objective, (const void*)Zn);
        *optimizer = BL_cg_new(
            constraints->vardim, 1.0e-4, XV0, TEST_lagrange, (const void*)constraints
        );
    } while (0);
    return err;
}

void TEST_destroy(pBL_cg_t* ppoptimizer)
{
    if (!ppoptimizer || !(*ppoptimizer))
    {
        return;
    }
    pBL_constraint_t constraints = (pBL_constraint_t)((*ppoptimizer)->params);
    BL_SAFEFREE(&constraints->eqc);
    BL_SAFEFREE(&constraints->neqc);
    BL_SAFEFREE(ppoptimizer);
}

int TEST_create_penalty(pcTEST_plane_t plane, pcTEST_cylinder_t cylinder, pBL_cg_t* optimizer)
{
    static const BL_3r32_t Zn = {0.0f, 0.0f, 1.0f};
    // static const BL_1r32_t XV0[] = {0.71f, 1.0f, 1.3f};
    static const BL_1r32_t XV0[] = {0.7070f, 1.00902f, 1.29278f};
    static const BL_1r32_t penalty_factor = 1.0e6f;
    int err = ESUCCESS;
    pBL_constraint_t constraints = NULL;
    if (!fnmat) { fnmat = BL_matfnr32RM(); }
    do {
        if (!optimizer || *optimizer)
        {
            err = EINVAL;
            break;
        }
        pBL_array_t eqc = BL_array_new(2, BL_3uptr_t);
        pBL_array_t neqc = BL_array_new(0, BL_3uptr_t);
        BL_ptr_t i_eqc = BL_array_begin(eqc);
        i_eqc._3uptr[0][0] = (BL_1uptr_t)TEST_eqc0;
        i_eqc._3uptr[0][1] = (BL_1uptr_t)plane;
        i_eqc._3uptr[0][2] = (BL_1uptr_t)penalty_factor;
        i_eqc._3uptr[1][0] = (BL_1uptr_t)TEST_eqc1;
        i_eqc._3uptr[1][1] = (BL_1uptr_t)cylinder;
        i_eqc._3uptr[1][2] = (BL_1uptr_t)penalty_factor;
        constraints = BL_constraint_new(eqc, neqc, 3, TEST_objective, (const void*)Zn);
        *optimizer = BL_cg_new(
            3, 1.0e-3f, XV0, TEST_penalty, (const void*)constraints
        );
    } while (0);
    return err;
}
