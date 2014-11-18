// any.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef ANY_H
#define ANY_H 1

#include <typeinfo>

namespace Ra
{
    class Any
    {
    public:
        template < class T >
        Any(const T& value)
            : _content(new Holder<T>(value))
        {}

        Any(const Any& rhs)
            : _content(rhs._content ? rhs._content->clone() : 0)
        {}

        ~Any()
        {
            delete _content;
        }

        template < class T >
        Any& operator=(const T& value)
        {
            delete _content;
            _content = new Holder<T>(value);
            return *this;
        }        

        Any& operator=(const Any& rhs)
        {
            delete _content;
            _content = rhs._content ? rhs._content->clone() : 0;
            return *this;
        }

        const std::type_info& type() const
        {
            return _content ? _content->type() : typeid(void);
        }

        template < class T >
        const T& cast()
        {
            return dynamic_cast< Holder<T>& >(*_content)._value;                
        }
        
    private:
        struct HolderBase
        {
            virtual ~HolderBase() {}
            virtual HolderBase* clone() const = 0;
            virtual const std::type_info& type() const = 0;
        };

        template < class T >
        struct Holder : public HolderBase
        {
            T _value;
            Holder (const T& value) : _value(value) {}

            virtual HolderBase* clone() const
            {
                return new Holder(_value);
            }

            virtual const std::type_info& type() const
            {
                return typeid(T);
            }
        };

        HolderBase* _content;

        
    };
} // namespace Ra


#endif // ANY_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
