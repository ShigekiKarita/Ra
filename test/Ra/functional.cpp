#include <gtest/gtest.h>

#include "../../src/Ra/functional.hpp"

namespace Ra
{
    namespace TestFunctional
    {
    
        int func() { return 3; };

        TEST(Functinal, function)
        {
            auto fun = [](){ return 4; };

            function<int()> f;

            f = &func;
            ASSERT_EQ(3, f());

            f = fun;
            ASSERT_EQ(4, f());
        }

    }
}
