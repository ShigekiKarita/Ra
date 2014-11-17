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

    template < class T >
    class recursive_wrapper
    {
    private:
        T* _t;

        template < class U >
        void assign(U&& u)
        {
            *_t = std::forward<U>(u);
        }

        template < class U >        
        using is_convertible = typename
            std::enable_if< std::is_convertible<U, T>::value, U >::type;

    public:
        template < class U, class _ = is_convertible<U> >
        recursive_wrapper(const U& u) : _t(new T(u))
        {}
        
        template < class U, class _ = is_convertible<U> >
        recursive_wrapper(U&& u) : _t(new T(std::forward<U>(u)))
        {}
        
        recursive_wrapper(const recursive_wrapper& rhs) : _t(new T(rhs.get()))
        {}

        recursive_wrapper(recursive_wrapper&& rhs) : _t(rhs._t)
        { rhs._t = nullptr; }        

        ~recursive_wrapper()
        { delete _t; }
        
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
        template < class T, class Internal >
        T& get_value(T& t, const Internal&) { return t; }

        template < class T >
        T& get_value(recursive_wrapper<T>& t, const std::false_type&)
        {
            return t.get();
        }

        template < class T >
        const T& get_value(const recursive_wrapper<T>& t, const std::false_type&)
        {
            return t.get();
        }
    } // namespace detail


    template < class Internal, class T, class Storage, class Visitor,
               class ... Args >
    class Visitor::result_type visitor_caller
    (Internal&& internal, Storage&& storage, Visitor&& visitor, Args&& ... args)
    {
        using ConstType =
            class std::conditional<
                std::is_const<
                    class std::remove_extent<
                        class std::remove_reference<Storage>::type
                    >::type
                >::value,
                const T,
                T
            >::type;

        return visitor(detail::get_value(storage, internal),
                       std::forward<Args>(args) ... );
    }


    // TOO COMPLEX and huge ... maybe I need simpler one
    template < class Head, class ... Rest >
    class Variant
    {
    private:        
        static constexpr auto _size = Atum::max_sizeof<Head, Rest ...>;
        using _align = Atum::max_alignof< Head, Rest ... >;
        
        union
        {
            char _storage[_size];
        };

        
        template < class ... Ts >
        struct do_visit
        {
            template < class Internal, class Storage, class Visitor, class ... Args >
            class Visitor::result_type operator()
            (Internal && internal, const std::size_t index, Storage&& storage,
             Visitor&& visitor, Args&& ... args)
            {
                static_assert(index < sizeof ... (Ts), "out of range in visitor");
                
                using Caller = class Visitor::result_type (*)
                    (Internal&&, Storage&&, Visitor&&, Args&&...);

                static Caller callist[sizeof...(Ts)] =
                    { &visitor_caller<Internal&&, Ts, Storage&&, Visitor, Args&& ... > ... };

                return (*callist[index])(std::forward(internal),
                                         std::forward(storage),
                                         std::forward(visitor),
                                         std::forward(args) ... );
            }
        };

        
        class Constructor
        {
        private:
            Variant& _self;

        public:
            using result_type = void;

            Constructor(Variant& self) : _self(self) {}

            template < class T >
            void operator () (const T& rhs) const
            { _self.construct(rhs); }
        };

        
        class MoveConstructor
        {
        private:
            Variant& _self;

        public:
            using result_type = void;

            MoveConstructor(Variant& self) : _self(self) {}

            template < class T >
            void operator () (const T& rhs) const
            { _self.construct(std::move(rhs)); }            
        };
        
        template <typename T>
        void construct(T&& t)
        {
            using type = typename std::remove_reference<T>::type;
            new (_storage) type(std::forward<T>(t));
        }
        

        void indicate_index(std::size_t index)
        { _index = index; }

        std::size_t _index;


        template < class Visitor >
        typename Visitor::result_type
        apply_visitor_internal(Visitor& visitor)
        {
            return apply_visitor<std::true_type, Visitor>(visitor);
        }

        template < class Visitor >
        typename Visitor::result_type
        apply_visitor_internal(Visitor& visitor) const
        {
            return apply_visitor<std::true_type, Visitor>(visitor);
        }
                
        struct Destroyer
        {
            using result_type = void;

            template < class T >
            void operator()(T& t) const
            { t.~T(); }
        };

        
        void destroy()
        {
            Destroyer d;
            // apply_visitor_internal(d);
        }
        
    public:
        Variant()
        {
            construct(Head());
            indicate_index(0);
        }

        ~Variant()
        { destroy(); }

        template < class T >
        Variant(T&& t);

        template < class Internal, class Visitor, class ... Args>
        typename Visitor::result_type
        apply_visitor(Visitor& visitor, Args&&... args)
        {
            return do_visit< Head, Rest ...>()
                (Internal(), _index, _storage, visitor, std::forward<Args>(args)...);
        }

        template < class Internal, class Visitor, class ... Args>
        typename Visitor::result_type
        apply_visitor(Visitor& visitor, Args&&... args) const
        {
            return do_visit< Head, Rest ...>()
                (Internal(), _index, _storage, visitor, std::forward<Args>(args)...);
        }
    };


    template <typename Visitor, typename Visitable, typename... Args>
    typename Visitor::result_type
    apply_visitor(Visitor& visitor, Visitable& visitable, Args&&... args)
    {
        return visitable.template apply_visitor<std::false_type>
            (visitor, std::forward<Args>(args)...);
    }

    template <typename Visitor, typename Visitable, typename... Args>
    typename Visitor::result_type
    apply_visitor(const Visitor& visitor, Visitable& visitable, Args&&... args)
    {
        return visitable.template apply_visitor<std::false_type>
            (visitor, std::forward<Args>(args)...);
    }

    
} // namespace Ra


#endif // VARIANT_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//

