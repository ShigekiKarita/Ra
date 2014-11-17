// variant.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef VARIANT_H
#define VARIANT_H 1


struct _void {};

template < class Head = _void, class ... Rest >
struct typelist
{
    using list = typelist< Rest ..., _void >;
    constexpr static auto length = 1 + sizeof ... (Rest);
};


template <
    typename T1 = _void,
    typename T2 = _void,
    typename T3 = _void,
    typename T4 = _void
    >
struct typelist_traits
{
    using list = typelist_traits< T2, T3, T4, _void >;
    constexpr static auto length = 1 + list::length;    
};

template <>
struct typelist_traits < _void, _void, _void, _void >
{
    using list = _void;
    constexpr static auto length = 0;
};

#endif // VARIANT_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//

