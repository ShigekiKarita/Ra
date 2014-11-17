// variant.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef VARIANT_H
#define VARIANT_H 1

#include <functional>
#include "../Atum/atum.hpp"

namespace Ra
{

    template < typename T >
    class  recursive_wrapper
    {
    private:
        T* _t;

        template < typename U >
        void assign(U&& u)
        {
            *_t = std::forward<U>(u);
        }

        template < typename U >        
        using is_convertible = typename
            std::enable_if< std::is_convertible<U, T>::value, U >::type;

    public:
        template < typename U, typename _ = is_convertible<U> >
        recursive_wrapper(const U& u) : _t(new T(u)) {}
        
        template < typename U, typename _ = is_convertible<U> >
        recursive_wrapper(U&& u) : _t(new T(std::forward<U>(u))) {}
        
        recursive_wrapper(const recursive_wrapper& rhs) : _t(new T(rhs.get())) {}

        recursive_wrapper(recursive_wrapper&& rhs) : _t(rhs._t) { rhs._t = nullptr; }        

        ~recursive_wrapper() { delete _t; }
        
        recursive_wrapper& operator=(const recursive_wrapper& rhs)
        {
            assign(rhs.get());
            return *this;
        }

        recursive_wrapper& operator=(recursive_wrapper&& rhs)
        {
            delete _t;
            _t = rhs._t;
            rhs._t = nullptr;
            return *this;
        }

        recursive_wrapper& operator=(const T& t)
        {
            assign(t);
            return *this;
        }

        recursive_wrapper& operator=(T&& t)
        {
            assign(std::move(t));
            return *this;
        }

        T& get() { return *_t; }

        const T& get() const { return *_t; }
    };

    namespace detail
    {
        template < typename T, typename Internal >
        T& get_value(T& t, const Internal&) { return t; }

        template < typename T >
        T& get_value(recursive_wrapper<T>& t, const std::false_type&)
        {
            return t.get();
        }

        template < typename T >
        const T& get_value(const recursive_wrapper<T>& t, const std::false_type&)
        {
            return t.get();
        }
    } // namespace detail


    template < typename Internal, typename T, typename Storage, typename Visitor,
               typename ... Args >
    typename Visitor::result_type visitor_caller
    (Internal&& internal, Storage&& storage, Visitor&& visitor, Args&& ... args)
    {
        using ConstType =
            typename std::conditional<
                std::is_const<
                    typename std::remove_extent<
                        typename std::remove_reference<Storage>::type
                    >::type
                >::value,
                const T,
                T
            >::type;

        return visitor(detail::get_value(storage, internal),
                       std::forward<Args>(args) ... );
    }
    
    template < typename Head, typename ... Rest >
    class Variant
    {
    private:
        template < typename ... Ts >
        struct do_visit
        {
            template < typename Internal,
                       typename Storage,
                       typename Visitor,
                       typename ... Args >
            typename Visitor::result_type operator()
            (Internal && internal, const std::size_t index, Storage&& storage,
             Visitor&& visitor, Args&& ... args)
            {
                static_assert(index < sizeof ... (Ts), "out of range in visitor");
                
                using Caller = typename Visitor::result_type (*)
                    (Internal&&, Storage&&, Visitor&&, Args&&...);

                static Caller callist[sizeof...(Ts)] =
                    {
                        &visitor_caller<Internal&&, Ts, Storage&&, Visitor, Args&& ... > ...
                    };

                return (*callist[index])(std::forward(internal),
                                         std::forward(storage),
                                         std::forward(visitor),
                                         std::forward(args) ... );
            }
        };

        template <typename T>
        struct Sizeof
        {
            static constexpr size_t value = sizeof(T);
        };

        template <typename T>
        struct Alignof
        {
            static constexpr size_t value = alignof(T);
        };

        static constexpr auto _size =
            Atum::max_size<Sizeof, Head, Rest ...>::value;

        
    public:
        Variant() = default;

        template < typename T >
        Variant(T&& t);

        template <typename T>
        T& get(Variant&);
 
        template <typename T>
        const T& get(const Variant&);
 
        template <typename T>
        T* get(Variant*);
     
        template <typename T>
        const T* get(const Variant*);
    };

    template <typename Visitor, typename Visitable, typename... Args>
    typename Visitor::result_type                                                
    apply_visitor(Visitor& visitor, Visitable& visitable, Args&&... args);
 
    template <typename Visitor, typename Visitable, typename... Args>
    typename Visitor::result_type
    apply_visitor(const Visitor& visitor, Visitable& visitable, Args&&... args);


} // namespace Ra


#endif // VARIANT_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//

