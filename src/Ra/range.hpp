// range.hpp
//
// last-edit-by: karita
//
// Description: range interface
//
//////////////////////////////////////////////////////////////////////

#ifndef RANGE_H
#define RANGE_H 1

namespace Ra
{
    template <
        class Elem,
        class Array
        >
    class Range
    {
        virtual Elem next() = 0;
        virtual Elem prev() = 0;
        virtual Array to_array() = 0;
        ~Range() = delete;
    };


    // TODO: Impl as range
    template < class R, class F >
    R map(const R& input, const F& func)
    {
        R output;
        output.resize(input.size());
        
        auto o = output.begin();        
        for (auto i = input.begin(); i != input.end(); ++i)
        {
            *o = func(*i);
            ++o;
        }
        
        return output;
    }
    
    template < class R, class F >
    R filter(const R& range, const F& function)
    {
        
    }

    template < class R, class F >
    R reduce(const R& range, const F& function)
    {
        
    }
    
} // namespace Ra

#endif // RANGE_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
