#include <gtest/gtest.h>

#include "../../src/Atum/atum.hpp"

namespace Atum {
    
    struct A {
        double a;
        char c;
    };
    
    struct B {

    };
    

    TEST(Atum, max_value)
    {
        static_assert(max_sizeof<B, A> == 16);
    }

}
