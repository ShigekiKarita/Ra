// policy.hpp
//
// last-edit-by: karita
//
// Description: referrence
//
//////////////////////////////////////////////////////////////////////

#ifndef POLICY_H
#define POLICY_H 1

#include <fstream>

namespace supervisor
{

struct DebugLog
{
    static auto print(const std::string& value)
    {
        return value;
    }
};

struct ReleaseLog
{
    static auto print(const std::string& value)
    {
        return "";
    }
};

struct FileLog
{
    static void print(const std::string& value)
    {
        std::ofstream file("./log.txt");
        file << value << std::endl;
    }
};

template <class LogPolicy = DebugLog>
struct Out
{
    auto foo() const
    {
        return LogPolicy::print("message");
    }
};


// replace inheritance(2*3) with policy(2+3)
// on thread
class single_thread {};
class multi_thread {};
// on memory
class reference_count {};
class deep_copy {};
class copy_on_write {};


template <class Thread, class Memory>
class smart_ptr {};

} // namespace supervisor

#endif // POLICY_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
