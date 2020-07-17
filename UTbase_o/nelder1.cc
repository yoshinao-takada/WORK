#include    <gtest/gtest.h>
#include    "base_o/BL_nelder1.h"
#include    "UTbase_o/enable_items.h"
#define SUIT    nelder1
namespace
{
#if defined(ENABLE_NELDER1)
    TEST(SUIT, run_convergence1)
    {
        pBL_nelder1_t p = BL_nelder1_new(BL_nelder1_testfunc, nullptr, 2.5e1f, 1.0e-6f);
        ASSERT_NE(nullptr, p);
        int err = BL_nelder1_run(p, 100);
    }
#endif
}
