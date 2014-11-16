// tuple.hpp
//
// last-edit-by: karita
//
// Description:
//
//////////////////////////////////////////////////////////////////////

/// TODO: iostream overload
/// TODO: survey initializer_list constructor in std::tuple

#ifndef TUPLE_H
#define TUPLE_H 1

#include <typeinfo>

namespace Ra 
{
    namespace detail
    {
        // tuple
        template < std::size_t, class ... >
        struct tuple_impl;

        template < std::size_t N >
        struct tuple_impl < N > {};

        template < std::size_t N, class Head, class ... Rest >
        struct tuple_impl < N, Head, Rest ... >
            :  tuple_impl < N + 1, Rest ... >
        {
            Head value;
        };        
    } //namespace detail

    // alias template never hides impl
    template < class ... Ts >
    struct tuple : detail::tuple_impl < 0, Ts ... > {};

    template < class ... Ts >
    constexpr auto& get(tuple < Ts ... >& t)
    {
        return t.value;
    }

    template < class ... Ts >
    constexpr auto length(const tuple < Ts ... >&)
    {
        return sizeof...(Ts);
    }
}


#endif // TUPLE_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
