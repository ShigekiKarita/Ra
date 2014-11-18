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


    namespace detail
    {
        union AnyPointer
        {
            void (*func_ptr)();
            void* func_obj;
        };

        template < class Func, class Result >
        struct FunctionPtrManager
        {
            static Result invoke(AnyPointer any_ptr)
            {
                Func func = reinterpret_cast<Func>(any_ptr.func_ptr);
                return func();
            }

            static void destroy(AnyPointer) {};
        };

        template < class Func, class Result >
        struct FunctionObjManager
        {
            static Result invoke(AnyPointer any_ptr)
            {
                Func* func = reinterpret_cast<Func*>(any_ptr.func_obj);
                return (*func)();
            }

            static void destroy(AnyPointer any_ptr)
            {
                Func* func = reinterpret_cast<Func*>(any_ptr.func_obj);
                delete func;
            }
        };
    } // namespace detail
    
    template < class T >
    class function;

    template < class Result >
    class function < Result() >
    {
    private:
        Result (*_invoke) (detail::AnyPointer);
        void (*_destroy) (detail::AnyPointer);
        detail::AnyPointer _functor;

        void clear()
        {
            if (_destroy)
            {
                _destroy(_functor);
            }
            _destroy = 0;
        }
        
    public:
        function()
            : _invoke(0), _destroy(0)
        {}

        ~function()
        {
            clear();
        }

        Result operator() ()
        {
            return _invoke(_functor);
        }
        
        template < class Func >
        void set_ptr(Func func_ptr)
        {
            clear();
            _invoke = &detail::FunctionPtrManager<Func, Result>::invoke;
            _destroy = &detail::FunctionPtrManager<Func, Result>::destroy;
            _functor.func_ptr = reinterpret_cast<void(*)()>(func_ptr);
        }
       template < class Func >
        void set_obj(Func func_obj)
        {
            clear();
            _invoke = &detail::FunctionObjManager<Func, Result>::invoke;
            _destroy = &detail::FunctionObjManager<Func, Result>::destroy;
            _functor.func_obj = reinterpret_cast<void*>(new Func(func_obj));
        }
        
    };

    template < class R, class Arg >
    class function < R(Arg) >
    {};

    
    
} // namespace Ra

#endif // FUNCTOR_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
