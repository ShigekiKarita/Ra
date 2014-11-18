#include <iostream>

#include <gtest/gtest.h>

#include "../../src/Ra/any.hpp"

namespace Ra
{
    struct Hoge
    {
        int x = -1;
        ~Hoge() {
            std::cout << "delete Hoge\n";
        }
    };
    
    TEST(Any, init)
    {
        Any val = 3;
        ASSERT_EQ(typeid(int), val.type());
        ASSERT_EQ(3, val.cast<int>());
        
        val = Hoge();
        ASSERT_EQ(typeid(Hoge), val.type());        
    }
    
} // namespace Ra
