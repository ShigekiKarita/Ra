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


    template < class Internal, class T, class Storage, class Visitor, class ... Args >
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

        return visitor(detail::get_value(*reinterpret_cast<ConstType*>(storage), internal),
                       std::forward<Args>(args) ... );
    }


    // TOO COMPLEX and huge ... maybe I need simpler one
    template < class Head, class ... Rest >
    class Variant
    {
    private:        
        using _align = Atum::max_alignof< Head, Rest ... >;

        static constexpr auto _size = Atum::max_sizeof<Head, Rest ...>;
        
        union
        {
            char _storage[_size];
        };

        std::size_t _index;
        
        void set_index(std::size_t index)
        {
            _index = index;
        }

        
        template <typename... AllTypes>
        struct do_visit
        {
            template < class Internal, class Storage, class Visitor, class... Args >                
            typename Visitor::result_type
            operator()
            (Internal&& internal, std::size_t index, Storage&& storage, Visitor& visitor, Args&&... args)
            {                
                using Caller = typename Visitor::result_type (*)
                    (Internal&&, Storage&&, Visitor&&, Args&&...);

                static Caller callers[sizeof...(AllTypes)] =
                    {
                        &visitor_caller<Internal&&, AllTypes, Storage&&, Visitor, Args&&...>...
                    }; 

                assert(index < sizeof...(AllTypes));

                return (*callers[index])
                    (std::forward<Internal>(internal),
                     std::forward<Storage>(storage), 
                     std::forward<Visitor>(visitor), 
                     std::forward<Args>(args)...);
            }
        };


        
        class Constructor
        {
        private:
            Variant& _self;

        public:
            using result_type = void;

            Constructor(Variant& self) : _self(self)
            {}

            template < class T >
            void operator () (const T& rhs) const
            {
                _self.construct(rhs);
            }
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
            {
                _self.construct(std::move(rhs));
            }
        };
        
        template <typename T>
        void construct(T&& t)
        {
            using type = typename std::remove_reference<T>::type;
            new (_storage) type(std::forward<T>(t));
        }

        
        struct Assigner
        {
            typedef void result_type;

            Assigner(Variant& self, int rhs_which)
                : m_self(self), m_rhs_which(rhs_which)
            {
            }

            template <typename Rhs>
            void operator()(const Rhs& rhs) const
            {
                if (m_self.which() == m_rhs_which)
                {
                    //the types are the same, so just assign into the lhs
                    *reinterpret_cast<Rhs*>(m_self.address()) = rhs;
                }
                else
                {
                    Rhs tmp(rhs);
                    m_self.destroy(); //nothrow
                    m_self.construct(std::move(tmp)); //nothrow (please)
                }
            }

        private:
            Variant& m_self;
            int m_rhs_which;
        };
  
        struct MoveAssigner
        {
            typedef void result_type;

            MoveAssigner(Variant& self, int rhs_which)
                : m_self(self), m_rhs_which(rhs_which)
            {
            }

            template <typename Rhs>
            void operator()(Rhs& rhs) const
            {
                typedef typename std::remove_const<Rhs>::type RhsNoConst;
                if (m_self.which() == m_rhs_which)
                {
                    //the types are the same, so just assign into the lhs
                    *reinterpret_cast<RhsNoConst*>(m_self.address()) = std::move(rhs);
                }
                else
                {
                    m_self.destroy(); //nothrow
                    m_self.construct(std::move(rhs)); //nothrow (please)
                }
            }
            
        private:
            Variant& m_self;
            int m_rhs_which;
        };
        
       
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
            {
                t.~T();
            }
        };
        
        void destroy()
        {
            Destroyer d;
            apply_visitor_internal(d);
        }

        /// FIXME: Oh, this is tuple
        template < std::size_t index, class ... Types >
        struct Initializer;

        template < std::size_t index, class Current, class ... Types >
        struct Initializer < index, Current, Types ... >
            : public Initializer < index + 1, Types ... >
        {
            using Base = Initializer < index + 1, Types ... >;

            static void init(Variant& v, Current&& c)
            {
                v.construct(std::forward<Current>(c));
                v.set_index(index);
            }

            static void init(Variant& v, const Current& c)
            {
                v.construct(c);
                v.set_index(index);
            }
        };
        
    public:
        Variant()
        {
            construct(Head());
            set_index(0);
        }

        ~Variant()
        {
            destroy();
        }

        template <
            class T ,
            class _ = typename std::enable_if
            <
                !std::is_same
                <
                    typename std::remove_reference<Variant<Head, Rest ...>>::type,
                    typename std::remove_reference<T>::type
                >::value,
                T
            >::type
        >
        Variant(T&& t)
        {
            static_assert(!std::is_same< Variant<Head, Rest ...>, T>::value,
                          "Error: fail to instatiate with Variant(T&&)");
            
            Initializer<0, Head, Rest ...>::init(*this, std::forward<T>(t));
        }

        Variant(const Variant& rhs)
        {
            Constructor c(*this);
            rhs.apply_visitor_internal(c);
            set_index(rhs.which());
        }

        Variant(Variant&& rhs)
        {
            MoveConstructor mc(*this);
            rhs.apply_visitor_internal(mc);
            set_index(rhs.which());
        }

        Variant& operator=(const Variant& rhs)
        {
            if (this != &rhs)
            {
                Assigner a(*this, rhs.which());
                rhs.apply_visitor_internal(a);
                set_index(rhs.which());
            }
            return *this;
        }

        Variant& operator=(Variant&& rhs)
        {
            if (this != &rhs)
            {
                MoveAssigner ma(*this, rhs.which());
                rhs.apply_visitor_internal(ma);
                set_index(rhs.which());
            }
            return *this;
        }

        std::size_t which() const {return _index;}

        
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

