// matrix.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef MATRIX_H
#define MATRIX_H 1

#include <initializer_list>
#include <cstddef>
#include <functional>

namespace Ra
{
    template< class T, const std::size_t N >
    class Vector
    {
    public:
        typedef T value_type;
                
        constexpr static auto length = N;
                
        Vector()
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                values_[i] = 0;
            }
        }
                
        Vector(const std::initializer_list<T>& il)
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                values_[i] = il[i];
            }
        }
                
        Vector(const Vector& rhs)
        {
            // std::cout << "I am copy-constructor.\n";
            for (std::size_t i = 0; i < N; ++i)
            {
                values_[i] = rhs[i];
            }
        }
                
        template <class E>
        auto& operator+=(const E& rhs)
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                values_[i] += rhs[i];
            }
            return *this;
        }
                
        template <class E>
        auto& operator*=(const E& rhs)
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                values_[i] *= rhs[i];
            }
            return *this;
        }
                
        template <class E>
        auto& operator=(const E& rhs)
        {
            for (std::size_t i = 0; i < length; ++i)
            {
                values_[i] = rhs[i];
            }
            return *this;
        }
                
        T& operator[](std::size_t i)
        {
            return values_[i];
        }
                
        T operator[](std::size_t i) const
        {
            return values_[i];
        }
                
    private:
        T values_[N];
    };

    template < class ElemType >
    class Matrix; // TODO
            
    template < class L, class Op, class R >
    class expression
    {
        const L& _l;
        const R& _r;
    public:
        expression(const L& l, const R& r)
            : _l(l), _r(r) {}
                
        auto operator[](std::size_t i) const -> decltype(_l[i])
        {
            return Op(_l[i], _r[i]);
        }
    };
                        
    template< class L, class R >
    auto operator+(const L& lhs, const R& rhs)
    {
        return [](const auto l, const auto r) { return l + r; };
        // expression<L, std::plus, R>(lhs, rhs);
    }
            
    template< class L, class R >
    auto operator*(const L& lhs, const R& rhs)
    {
        return [](const auto l, const auto r) { return l * r; };
        // expression<L, std::multiplies, R>(lhs, rhs);
    }
} // namespace Ra


#endif // MATRIX_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
