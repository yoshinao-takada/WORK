#include    <gtest/gtest.h>
#include    "base_o/BL_penalty.h"
#define SUIT    optc

namespace
{
    bool EqualF(float f0, float f1, float tol)
    {
        float abssum = fabsf(f0 + f1);
        if (abssum < tol)
        {
            f0 += tol;
            f1 += tol;
        }
        abssum = fabsf(f0 + f1);
        float absdiff = fabsf(f0 - f1);
        return (absdiff/abssum) < tol;
    }

    static int objective(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }

    static int eqc0(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }

    static int eqc1(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }
    static int eqc2(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }
    static int neqc0(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }
    static int neqc1(uint32_t vardim, const BL_1r32_t* xv, const void* params, BL_1r32_t* pf)
    {
        return 0;
    }
    const BL_OBJECTIVE_FUNC eqc_funcs[] = { eqc0, eqc1, eqc2 };
    const BL_OBJECTIVE_FUNC neqc_funcs[] = { neqc0, neqc1 };
    const void* objective_params = (const void*)0xaa;
    const void* eqc_params[] = { (const void*)0x10, (const void*)0x11, (const void*)0x12 };
    const void* neqc_params[] = { (const void*)0x20, (const void*)0x21 };
    const BL_2r32_t objective_weight = { 1.0f, 1.0f };
    const BL_2r32_t eqc_weight[] = {
        { 1.0f, 10.0f },
        { 1.1f, 11.0f },
        { 1.2f, 12.0f }
    };
    const BL_2r32_t neqc_weight[] = {
        { 2.0f, 20.0f },
        { 2.1f, 21.0f }
    };

    TEST(SUIT, set_get)
    {
        // create an object as optc
        pBL_optc_t optc = BL_optc_new(ARRAYSIZE(eqc_funcs), ARRAYSIZE(neqc_funcs));
        ASSERT_EQ(3, optc->n_eqc);
        ASSERT_EQ(2, optc->n_neqc);
        ASSERT_EQ(6, optc->objectives->unit_count);

        // set objective and constraints
        BL_optc_element_t element_set = {
            objective, objective_params, { objective_weight[0], objective_weight[1] }
        };
        BL_optc_element_t element_get;
        ASSERT_EQ(ESUCCESS, BL_optc_set(optc, 0, &element_set));
        for (uint32_t i = 0; i != ARRAYSIZE(eqc_funcs); i++)
        {
            element_set.func = eqc_funcs[i];
            element_set.params = eqc_params[i];
            BL_copy2(element_set.weight, eqc_weight[i]);
            ASSERT_EQ(ESUCCESS, BL_OPTC_SET_EQC(optc, i, &element_set));
        }
        for (uint32_t i = 0; i != ARRAYSIZE(neqc_funcs); i++)
        {
            element_set.func = neqc_funcs[i];
            element_set.params = neqc_params[i];
            BL_copy2(element_set.weight, neqc_weight[i]);
            ASSERT_EQ(ESUCCESS, BL_OPTC_SET_NEQC(optc, i, &element_set));
        }

        // confirm objective and constraints
        ASSERT_EQ(ESUCCESS, BL_optc_get(optc, 0, &element_get));
        ASSERT_EQ((const void*)objective, (const void*)element_get.func);
        ASSERT_EQ(objective_params, element_get.params);
        ASSERT_EQ(objective_weight[0], element_get.weight[0]);
        ASSERT_EQ(objective_weight[1], element_get.weight[1]);
        for (uint32_t i = 0; i != ARRAYSIZE(eqc_funcs); i++)
        {
            ASSERT_EQ(ESUCCESS, BL_OPTC_GET_EQC(optc, i, &element_get));
            ASSERT_EQ((const void*)eqc_funcs[i], (const void*)element_get.func);
            ASSERT_EQ(eqc_params[i], element_get.params);
            ASSERT_EQ(eqc_weight[i][0], element_get.weight[0]);
            ASSERT_EQ(eqc_weight[i][1], element_get.weight[1]);
        }
        for (uint32_t i = 0; i != ARRAYSIZE(neqc_funcs); i++)
        {
            ASSERT_EQ(ESUCCESS, BL_OPTC_GET_NEQC(optc, i, &element_get));
            ASSERT_EQ((const void*)neqc_funcs[i], (const void*)element_get.func);
            ASSERT_EQ(neqc_params[i], element_get.params);
            ASSERT_EQ(neqc_weight[i][0], element_get.weight[0]);
            ASSERT_EQ(neqc_weight[i][1], element_get.weight[1]);
        }

        BL_optc_update(optc);
        const float tol = 1.0e-7f;
        for (uint32_t i = 0; i != ARRAYSIZE(eqc_funcs); i++)
        {
            ASSERT_EQ(ESUCCESS, BL_OPTC_GET_EQC(optc, i, &element_get));
            ASSERT_EQ((const void*)eqc_funcs[i], (const void*)element_get.func);
            ASSERT_EQ(eqc_params[i], element_get.params);
            ASSERT_TRUE(EqualF(eqc_weight[i][0]*eqc_weight[i][1], element_get.weight[0], tol));
            ASSERT_EQ(eqc_weight[i][1], element_get.weight[1]);
        }
        for (uint32_t i = 0; i != ARRAYSIZE(neqc_funcs); i++)
        {
            ASSERT_EQ(ESUCCESS, BL_OPTC_GET_NEQC(optc, i, &element_get));
            ASSERT_EQ((const void*)neqc_funcs[i], (const void*)element_get.func);
            ASSERT_EQ(neqc_params[i], element_get.params);
            ASSERT_TRUE(EqualF(neqc_weight[i][0]*neqc_weight[i][1], element_get.weight[0], tol));
            ASSERT_EQ(neqc_weight[i][1], element_get.weight[1]);
        }

        // delete the object
        BL_optc_delete(&optc);
    }


}