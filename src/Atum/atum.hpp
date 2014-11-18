// atum.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef ATUM_H
#define ATUM_H 1

#include <type_traits>

namespace Atum
{
    namespace detail
    {
        template < template <class> class Scale, class Prev, class ... >
        struct max_value_impl;

        template < template <class> class Scale, class Prev >
        struct max_value_impl< Scale, Prev >
        {
            static constexpr auto value = Scale<Prev>::value;
            using type = Prev;
        };

        template < template <class> class Scale, class Prev, class Next, class ... Args >
        struct max_value_impl< Scale, Prev, Next, Args ... >
        {
        private:
            using _next = typename std::conditional
            <
                (Scale<Next>::value > Scale<Prev>::value),
                max_value_impl<Scale, Next, Args...>,
                max_value_impl<Scale, Prev, Args...>
            >::type;
            
        public:
            static constexpr auto value = _next::value;
            using type = typename _next::type;

        };
    } // namespace detail
    
    template < class T >
    struct Sizeof
    { static constexpr auto value = sizeof(T); };

    template < class Head, class... Rest >
    static constexpr auto max_sizeof = detail::max_value_impl<Sizeof, Head, Rest... >::value;


    template < class T >
    struct Alignof
    { static constexpr auto value = alignof(T); };
    
    template < class Head, class... Rest >
    using max_alignof = typename detail::max_value_impl<Alignof, Head, Rest... >::type;
}


#endif // ATUM_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
