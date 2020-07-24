#include    <gtest/gtest.h>
#include    <base_o/BL_dfo.h>
#include    "UTbase_o/enable_items.h"

#define     SUIT    dfo
namespace
{
#if defined(ENABLE_DFO)
#if 0
    TEST(SUIT, compare_varfuncpair)
    {
        BL_varfuncpair_t vf0 = { new float[11], 0.1f }, vf1 = { new float[11], 0.5f };
        int cmp = compare_varfuncpair((const void*)&vf0, (const void*)&vf1);
        ASSERT_EQ(-1, cmp);
        cmp = compare_varfuncpair((const void*)&vf1, (const void*)&vf0);
        ASSERT_EQ(1, cmp);
        vf1.f = 0.1f;
        cmp = compare_varfuncpair((const void*)&vf1, (const void*)&vf0);
        ASSERT_EQ(0, cmp);
    }

    static int test_func0(uint32_t vardim, const BL_1r32_t* x, const void* params, BL_1r32_t* f)
    {
        int err = ESUCCESS;
        do {
            *f = 0.0f;
            for (uint32_t i=0; i != vardim; i++)
            {
                *f += (1.0f + (float)i) * x[i] * x[i];
            }
        } while (false);
        return err;
    }

    TEST(SUIT, sort_varfunc_)
    {
        uint32_t vardim = 8;
        float xv0[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        float keys[] = { 0.0f, 1.0f, -1.0f, 2.0f, -2.0f, 3.0f, -3.0f, 4.0f, -4.0f };
        int keys_sorted_index[] = { 8, 6, 4, 2, 0, 1, 3, 5, 7 };
        pBL_nelderMD_t p = BL_nelderMD_new(vardim, xv0, 1.0e-4f, test_func0, nullptr);
        for (uint32_t i = 0; i != vardim+1; i++)
        {
            p->xf[i].f = keys[i];
        }
        sort_varfunc(p);
        for (uint32_t i = 0; i != vardim + 1; i++)
        {
            ASSERT_EQ(keys[keys_sorted_index[i]], p->xf[i].f);
        }
        BL_SAFEFREE(&p);
    }

    TEST(SUIT, is_matched_segment_)
    {
        uint32_t vardim = 8;
        float xv0[] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
        pBL_nelderMD_t p = BL_nelderMD_new(vardim, xv0, 1.0e-4f, test_func0, nullptr);
        BL_varfuncpair_t vftarget = { nullptr, p->xf[3].f };
        for (uint32_t i = 0; i != vardim; i++)
        {
            if (is_matched_segment(&p->xf[i], &vftarget))
            {
                ASSERT_EQ(3, i);
            }
            else
            {
                ASSERT_NE(3, i);
            }            
        }
        vftarget.f += 0.5f;
        for (uint32_t i = 0; i != vardim; i++)
        {
            if (is_matched_segment(&p->xf[i], &vftarget))
            {
                ASSERT_EQ(3, i);
            }
            else
            {
                ASSERT_NE(3, i);
            }            
        }
        vftarget.f += 0.6f;
        for (uint32_t i = 0; i != vardim; i++)
        {
            if (is_matched_segment(&p->xf[i], &vftarget))
            {
                ASSERT_EQ(4, i);
            }
            else
            {
                ASSERT_NE(4, i);
            }            
        }
        BL_SAFEFREE(&p);
    }
    
#endif
    static int DISTANCE_FROM(uint32_t vardim, const BL_1r32_t *x, const void* params, BL_1r32_t* pf)
    {
        const BL_1r32_t *params_ = (const BL_1r32_t*)params;
        *pf = 0.0f;
        for (uint32_t i = 0; i != vardim; i++, params_ += 2)
        {
            BL_1r32_t weighted_diff = (x[i] - *params_)/(*(params_ + 1));
            *pf += weighted_diff * weighted_diff;
        }
        return 0;
    }

#if 0
    TEST(SUIT, nelderMD_centroid)
    {
        const BL_1r32_t xv0[] = { 10.0f, 0.0f, 1.0f };
        const float params_[] = { 2.0f, 0.5f, 2.0f, 0.5f, 2.0f, 1.5f };
        pBL_nelderMD_t p = BL_nelderMD_new(
            3, xv0, 1.0e-5f, DISTANCE_FROM, (const void*)params_);
        ASSERT_NE(nullptr, p);
        BL_varfuncpair_t varfunc;
        BL_nelderMD_centroid(p);
        BL_nelderMD_candidates(p->vardim, &(p->xf[p->vardim+1]), p->work);
        BL_nelderMD_eval_candidates(p);
        for (uint32_t i = 0; i <= (p->vardim+1); i++)
        {
            printf("p->xf[%d]: ", i);
            printf_varfuncpair(stdout, &(p->xf[i]), p->vardim);
        }
        for (uint32_t i = 0; i != BL_dfo_polytope_work_vertex_count; i++)
        {
            printf("work[%s]: ", BL_dfo_work_labels[i]);
            printf_varfuncpair(stdout, &(p->work[i]), p->vardim);
        }
        pBL_varfuncpair_t best_candidate = BL_nelderMD_find_best_candidate(p->work);
        printf("best candidate: ");
        printf_varfuncpair(stdout, best_candidate, p->vardim);
        ASSERT_EQ(ESUCCESS, insert(p->vardim+2, p->xf, best_candidate, p->vardim));
        for (uint32_t i = 0; i <= (p->vardim+1); i++)
        {
            printf("p->xf[%d]: ", i);
            printf_varfuncpair(stdout, &(p->xf[i]), p->vardim);
        }

        for (uint32_t i = 0; i != 70;i++)
        {
            BL_nelderMD_centroid(p);
            BL_nelderMD_candidates(p->vardim, &(p->xf[p->vardim+1]), p->work);
            BL_nelderMD_eval_candidates(p);
            for (uint32_t i = 0; i != BL_dfo_polytope_work_vertex_count; i++)
            {
                printf("work[%s]: ", BL_dfo_work_labels[i]);
                printf_varfuncpair(stdout, &(p->work[i]), p->vardim);
            }
            best_candidate = BL_nelderMD_find_best_candidate(p->work);
            printf("best candidate: ");
            printf_varfuncpair(stdout, best_candidate, p->vardim);
            ASSERT_EQ(ESUCCESS, insert(p->vardim+2, p->xf, best_candidate, p->vardim));
            for (uint32_t i = 0; i <= (p->vardim+1); i++)
            {
                printf("p->xf[%d]: ", i);
                printf_varfuncpair(stdout, &(p->xf[i]), p->vardim);
            }
        }
        BL_SAFEFREE(&p);
    }
#endif
    TEST(SUIT, nelderMD_run)
    {
        const BL_1r32_t xv0[] = { 1.0f, 1.0f, 1.0f };
        const float params_[] = { 2.0f, 0.5f, 2.0f, 0.5f, 2.0f, 1.5f };
        pBL_nelderMD_t p = BL_nelderMD_new(
            3, xv0, 1.0e-5f, DISTANCE_FROM, (const void*)params_);
        p->pf = nullptr;
        int result = BL_nelderMD_run(p, 60);
        printf("result = %d\n", result);
        for (uint32_t i = 0; i <= (p->vardim+1); i++)
        {
            printf("p->xf[%d]: ", i);
            printf_varfuncpair(stdout, &(p->xf[i]), p->vardim);
        }
        printf("eval_count = %d\n", p->count_eval_OF);
        BL_SAFEFREE(&p);
    }
#endif
}