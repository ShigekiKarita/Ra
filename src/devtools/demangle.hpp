// demangle.hpp
//
// last-edit-by: karita
//
// Description: only for debug
//
//////////////////////////////////////////////////////////////////////

#ifndef DEMANGLE_H
#define DEMANGLE_H 1

#include <cstdlib>
#include <typeinfo>
#include <iostream>

#include <cxxabi.h>


namespace devtools
{
    template<class T>
    const char* type_name = typeid(T).name();
    
    template<class T>
    const char* demangle =
        abi::__cxa_demangle(type_name<T>, 0, 0, nullptr);

    
    void printype() {}
    
    template < class Head, class ... Rest >
    void printype(Head head, Rest ... rest)
    {
        std::cout << "(" << demangle<Head> << ") " << head
                  << (sizeof ... (rest) > 0 ? ", " : "\n");
        
        print_type(rest ...);
    }

    
    class demangle
    {
    private :
        char * realname;

    public :        
        demangle(std::type_info const & ti)
        {
            int status = 0;
            realname = abi::__cxa_demangle( ti.name(), 0, 0, &status );
        }

        template < class T >
        demangle(const T& t) : demangle(typeid(decltype(t))) {}
        
        demangle(const demangle&) = delete;

        demangle& operator=(const demangle&) = delete;

        ~demangle()
        {
            std::free(realname);
        }

        operator char const * () const
        {
            return realname;
        }
    };
} // namespace devtools
    
#endif // DEMANGLE_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
