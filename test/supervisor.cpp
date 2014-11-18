#include <vector>
#include <gtest/gtest.h>

#include <supervisor/crtp.hpp>
#include <supervisor/lexical_cast.hpp>
#include <supervisor/policy.hpp>
#include <supervisor/concept.hpp>
#include <supervisor/type_traits.hpp>

using namespace supervisor;

TEST(NonCRTP, DynamicTemplateMethod)
{
    Base* base = new Derived();
    ASSERT_EQ("Derived::foo", base->foo());
    ASSERT_EQ("Derived::template_method", base->something());
    delete base;
}

TEST(CRTP, StaticTemplateMethod)
{
    DerivedCRTP derived;
    ASSERT_EQ("DerivedCRTP::foo", derived.base_foo());
    ASSERT_EQ("DerivedCRTP::template_method", derived.something());
}

TEST(CRTP, BartonNackmanTrick)
{
    Hoge h1(1);
    Hoge h2(1);
    ASSERT_TRUE(h1 == h2);

    Fuga f1(1);
    Fuga f2(2);
    ASSERT_TRUE(f1 != f2);
    ASSERT_TRUE(f1 < f2);
    ASSERT_TRUE(f1 <= f2);
    ASSERT_TRUE(f2 > f1);
    ASSERT_TRUE(f2 >= f1);
}

TEST(CRTP, enable_shared_from_this)
{
    std::shared_ptr<Shared> p(new Shared());
    p->setup_shared_from_this(p); // not need in std
    ASSERT_EQ("Shared", p->f());
}


TEST(TypeConv, LexicalCast)
{
    ASSERT_EQ(3, lexical_cast<int>("3"));
    ASSERT_EQ("123", lexical_cast<std::string>(123));

    int i = lexical_cast("123");
    ASSERT_EQ(123, i);

    std::string s = lexical_cast(123);
    ASSERT_EQ("123", s);
}


TEST(Policy, Logs)
{
    ASSERT_EQ("message", Out<DebugLog>().foo());
    ASSERT_EQ("", Out<ReleaseLog>().foo());
    ASSERT_EQ("message", Out<>().foo());

    // smart_ptr<single_thread, multi_thread> ptr1;
    // smart_ptr<multi_thread, deep_copy> ptr2;
}


TEST(Concept, Point)
{
    AnotherPoint a(0.0, 0.0);
    AnotherPoint b(3.0, 4.0);
    ASSERT_EQ(5.0, distance(a, b));
}

struct A {
    A& operator=(const A&) = delete;
};

struct B {};

TEST(TypeTraits, MetaFuncs)
{
    static_assert(is_void<void>::value, "");
    static_assert(!is_void<int>::value, "");

    static_assert(has_iterator<std::vector<int>>::value, "");
    static_assert(!has_iterator<int>::value, "");

    static_assert(true_type::value, "");
    static_assert(!false_type::value, "");

    //static_assert(!is_assignable<A>::value, "");
    //static_assert(is_assignable<B>::value, "");
}
