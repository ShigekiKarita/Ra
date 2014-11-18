#include <iostream>

#include <gtest/gtest.h>

#include <Ra/any.hpp>

namespace Ra
{
    namespace TestAny
    {
        bool is_delete = false;
        
        struct Hoge
        {
            int x = -1;
            ~Hoge() {
                is_delete = true;
            }
        };
    
        TEST(Any, init)
        {
            Any val = 3;
            ASSERT_EQ(typeid(int), val.type());
            ASSERT_EQ(3, val.cast<int>());
        
            val = Hoge();
            ASSERT_EQ(typeid(Hoge), val.type());

            val = nullptr;
            ASSERT_TRUE(is_delete);
        }
    } // namespace TestAny
} // namespace Ra
