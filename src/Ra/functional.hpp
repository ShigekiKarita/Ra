// functor.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef FUNCTOR_H
#define FUNCTOR_H 1


#include <type_traits>

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
        struct func_ptr_tag {};
        struct func_obj_tag {};

        template < typename Func >        
        using get_func_tag = typename std::conditional<
            std::is_pointer<Func>::value,
            func_ptr_tag,
            func_obj_tag
            >::type;
         
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

        template < class FuncPtr >
        void assign_to(FuncPtr func_ptr, detail::func_ptr_tag)
        {
            clear();
            _invoke = &detail::FunctionPtrManager<FuncPtr, Result>::invoke;
            _destroy = &detail::FunctionPtrManager<FuncPtr, Result>::destroy;
            _functor.func_ptr = reinterpret_cast<void(*)()>(func_ptr);
        }

        template < class FuncObj >
        void assign_to(FuncObj func_obj, detail::func_obj_tag)
        {
            clear();
            _invoke = &detail::FunctionObjManager<FuncObj, Result>::invoke;
            _destroy = &detail::FunctionObjManager<FuncObj, Result>::destroy;
            _functor.func_obj = reinterpret_cast<void*>(new FuncObj(func_obj));
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
        function& operator=(Func func)
        {
            assign_to(func, detail::get_func_tag<Func>());
            return *this;
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
