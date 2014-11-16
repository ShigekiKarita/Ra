// lexical_cast.hpp
//
// last-edit-by: karita
//
// Description: referrence
//
//////////////////////////////////////////////////////////////////////

#ifndef LEXICAL_CAST_H
#define LEXICAL_CAST_H 1

#include <sstream>

namespace supervisor
{

// no type inferrence
template <class To, class From>
To lexical_cast(From from)
{
    To result;
    std::stringstream ss;

    ss << from;
    ss >> result;

    return result;
}

// decltype of left value wow
template <class From>
class lexical_cast_t
{
private:
    From _from;

public:
    lexical_cast_t(From from) : _from(from) {}

    template <class To>
    operator To() const
    {
        return lexical_cast<To>(_from);
    }
};

template <class From>
lexical_cast_t<From> lexical_cast(From from)
{
    return lexical_cast_t<From>(from);
}

} // namespace supervisor

#endif // LEXICAL_CAST_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
