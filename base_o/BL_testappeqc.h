#ifndef BL_TESTAPPEQC_H_
#define BL_TESTAPPEQC_H_
#include    "base_o/BL_constraint.h"
#include    "base_o/BL_cg.h"
#ifdef __cplusplus
extern "C" {
#endif

int TEST_lagrange(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);



/*Note1: In world coord system, getting Z => Sample objective function
Point P(x,y,z), Z-axis normal Zn
z = <P, Zn>
\param vardim [in] must be equal to 3
\param x [in] P
\param params [in] BL_3r32_t { 0, 0, 1 } means z-axis uint vector, Zn
\param f [out] *f: Z-component of P
*/
int TEST_objective(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);

typedef struct {
    BL_3r32_t p0, vn;
} TEST_plane_t, *pTEST_plane_t;
typedef const TEST_plane_t *pcTEST_plane_t;
/*Note2: equality constraint function on a plane => Sample equality constraint 1
Point P0: plane reference point
Vector Vn: plane normal vector
Point Ps: subject point
f_eqc(Ps, param(P0,Vn))
    Vs = Ps - P0
    Vsn = <Vs, Vn> : normal vector component of Vs
    Vst = Vs - Vn*Vsn : plane tangential component of Vs
    f_eqc = Vsn/|Vst|
\param vardim [in] must be equal to 3
\param x [in] Ps
\param params [in] TEST_plane_t instance
\param f [out] *f = 0: on the plane, *f > 0: in Vn side half space, *f < 0 in -Vn side half space
*/
int TEST_eqc0(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);

typedef struct {
    BL_3r32_t axis0, axis_dir; // cylinder axis
    BL_1r32_t R; // radius
} TEST_cylinder_t, *pTEST_cylinder_t;
typedef const TEST_cylinder_t *pcTEST_cylinder_t;
/*Note3: equality constraint function on a cylinder => Sample equality constraint 2
Point P0: cylinder axis reference point
Vector Vd: cylinder axis unit direction vector
scalar R: cylinder radius
Point Ps: subject point
f_eqc(Ps, param(P0,Vd,R))
    foot of the normal line drawn from Ps to the cylinder axis P1 = P0 + <Ps - P0, Vd>Vd
    Radial vector from P1 to Ps: Vr = Ps - P1
    f_eqc is the difference of |Vr|^2 - R^2: f_eqc = <Ps - P1, Ps - P1> - R^2
\param vardim [in] must be equal to 3
\param x [in] Ps
\param params [in] TEST_cylinder_t instance
\param f [out] 0: on the cylinder surface, f > 0: outside cyilnder, f < 0: inside cylinder
*/
int TEST_eqc1(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f);

int TEST_create(pcTEST_plane_t plane, pcTEST_cylinder_t cylinder, pBL_cg_t* optimizer);

void TEST_destroy(pBL_cg_t* ppoptimizer);
#ifdef __cplusplus
}
#endif

#endif /* BL_TESTAPPEQC_H_ */