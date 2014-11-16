// functor.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_H
#define FUNCTOR_H 1


namespace Ra
{
    namespace detail
    {
        template < class Result, class Func >
        class functor_impl
        {
        private:
            Func _f;

        public:
            using result_type = Result;

            functor_impl(Func f) : _f(f) {}

            template < class T >
            result_type operator()(T& x) const
            {
                return _f(x);
            }

            template < class T >
            result_type operator()(const T& x) const
            {
                return _f(x);
            }
        };
    } // namespace detail

    template < class Result, class Func >
    auto functor(Func f)
    {
        return detail::functor_impl< Result, Func >{f};
    }
} // namespace Ra

#endif // FUNCTOR_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
