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
        template 
        <
            template <typename> class Size,
            typename SoFar, 
            typename... Args
            >
        struct max_size_impl;

        template 
        <
            template <typename> class Size,
            typename SoFar
            >
        struct max_size_impl<Size, SoFar>
        {
            static constexpr decltype(Size<SoFar>::value) value = 
                Size<SoFar>::value;
            using type = SoFar;
        };

        template 
        <
            template <typename> class Size,
            typename SoFar, 
            typename Next, 
            typename... Args
            >
        struct max_size_impl<Size, SoFar, Next, Args...>
        {
        private:
            using _next = typename std::conditional
            <
                (Size<Next>::value > Size<SoFar>::value),
                max_size_impl<Size, Next, Args...>,
                max_size_impl<Size, SoFar, Args...>
            >::type;
            
        public:
            static constexpr decltype(Size<SoFar>::value) value = _next::value;
            using type = typename _next::type;
        };
    }

    template <template <typename> class Size, typename... Args>
    struct max_size;

    template 
    <
        template <typename> class Size, 
        typename First, 
        typename... Args
        >
    struct max_size<Size, First, Args...>
    {
    private:
        using _impl = detail::max_size_impl< Size, First, Args ... >;

    public:
        static constexpr decltype(Size<First>::value) value = _impl::value;
        using type = typename _impl::type;
    };

    // template 
    // <
    //     template <typename> class Size, 
    //     typename First, 
    //     typename... Args
    //     >
    // static constexpr auto max_size =
    //     detail::max_size_impl< Size, First, Args ... >::value;
    
}


#endif // ATUM_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
