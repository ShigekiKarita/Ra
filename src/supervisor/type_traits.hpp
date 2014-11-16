// type_traits.hpp
//
// last-edit-by: karita
//
// Description: referrence
//
//////////////////////////////////////////////////////////////////////

#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H 1

namespace supervisor
{

template <class T>
struct is_void
{
    static const bool value = false;
};

template <> // specialized for void
struct is_void<void>
{
    static const bool value = true;
};


template <class T>
class has_iterator
{
private:
    template <class U>
    static constexpr bool check(typename U::iterator*)
    {
        return true;
    }

    template <class U>
    static constexpr bool check(...)
    {
        return false;
    }

public:
    static const bool value = check<T>(nullptr);
};

template <class T, T v>
struct integral_constant
{ static constexpr T value = v; };

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template<class T>
struct rvalue_ref
{ typedef T &&type; };

template<class T>
struct rvalue_ref<T &>
{ typedef T &&type; };

template<class T>
struct rvalue_ref<T &&>
{ typedef T &&type; };

template<class T>
typename rvalue_ref<T>::type declval();

//    struct is_assignable_impl
//    {
//        template<class T>
//        static auto check(T *) -> std::decltype(
//            std::declval<T &>() = std::declval<const T &>(), std::true_type::value
//        );
//
//        template<class T>
//        static auto check(...) -> std::false_type;
//    };
//
//    template<class T>
//    struct is_assignable : std::decltype
//            (is_assignable_impl::check<T>(nullptr))
//    {};

} // namespace supervisor

#endif // TYPE_TRAITS_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//

