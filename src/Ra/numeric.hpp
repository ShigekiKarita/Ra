// numeric.hpp
//
// last-edit-by: <> 
//
// Description:
//
//////////////////////////////////////////////////////////////////////

#ifndef NUMERIC_H
#define NUMERIC_H 1

namespace Ra
{
    template <class T>
    class equal_comparable {
        friend bool operator!=(const T& lhs, const T& rhs)
        {
            return !lhs.operator==(rhs);
        }
    };

    template < class T >
    class less_than_comparable {
        friend bool operator>(const T& lhs, const T& rhs)
        {
            return rhs.operator<(lhs);
        }

        friend bool operator<=(const T& lhs, const T& rhs)
        {
            return lhs.operator<(rhs) || lhs.operator==(rhs);
        }

        friend bool operator>=(const T& lhs, const T& rhs)
        {
            return !lhs.operator<(rhs);
        }
    };

    template <class T>
    class Ord : equal_comparable<T>, less_than_comparable<T>
    {};

    /** How 2 use
    class Fuga : private Ord<Fuga>
    {
        int _id;
    public:
        explicit Fuga(int id) : _id(id) {}

        bool operator==(const Fuga& rhs) const {
            return _id == rhs._id;
        }

        bool operator<(const Fuga& rhs) const {
            return _id < rhs._id;
        }
    };
    */
}


#endif // NUMERIC_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
