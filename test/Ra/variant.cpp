#include <gtest/gtest.h>
#include <Ra/variant.hpp>




TEST(Ra, Variant)
{
    Ra::Variant<int, char, bool> a;
}

class VisitorA
{
  public:
  typedef void result_type;
 
  void
  operator()(int);
 
  void
  operator()(char);
 
  void
  operator()(bool);
};
 
class VisitorB
{
  public:
  typedef void result_type;
 
  void
  operator()(int);
 
  template <typename T>
  void
  operator()(T&& t);
};
