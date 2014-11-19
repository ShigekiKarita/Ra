#include <gtest/gtest.h>
#include <Ra/variant.hpp>
#include <vector>

namespace Ra
{
    namespace TestVariant
    {

        TEST(Ra, Variant)
        {
            Ra::Variant<int, char, bool, std::vector<int>> a;
            a = 1;
            // ASSERT_EQ(1, a);
            a = 'a';
            // ASSERT_EQ('a', a);
            a = false;
            // ASSERT_EQ(false, a);
            // a = { 1, 2 };
        }

        /// TODO: Test visitors

        class VisitorA
        {
        public:
            typedef void result_type;
 
            void
            operator()(int);
 
            void
            operator()(char);
 
            void
            operator()(bool);
        };
 
        class VisitorB
        {
        public:
            typedef void result_type;
 
            void
            operator()(int);
 
            template <typename T>
            void
            operator()(T&& t);
        };

    }
}
