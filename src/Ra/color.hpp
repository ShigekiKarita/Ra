// color.hpp
//
// last-edit-by: karita
//
// Description:
//
//////////////////////////////////////////////////////////////////////

/// TODO: iostream overload

#ifndef COLOR_H
#define COLOR_H 1


#include <iostream>


namespace Ra
{
    enum class colors : std::uint8_t
    {
        black, red, green, yellow, blue, magenta, cyan, white, none
    };
 
    std::string to_string( const colors c )
    {
        return std::to_string(static_cast<std::uint8_t>(c));
    }
    
    void write() {}
 
    template < class T, class ... Ts >
    void write( const T & head, const Ts & ... rest )
    {
        std::cout << head;
        write(rest ...);
    }
    
    template < class ... Ts >
    void write( colors forground, const Ts ... content )
    {
        const auto prefix = "\x1b[3" + to_string(forground) + "m";
        write(prefix, content ..., "\x1b[m");
    }
 
    template < class ... Ts >
    void writeln( colors forground, const Ts & ... content )
    {
        write(forground, content ...);
        std::cout << std::endl;
    }
} // namespace Ra

#endif // COLOR_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
