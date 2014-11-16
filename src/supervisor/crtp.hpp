// crtp.hpp
//
// last-edit-by: karita
//
// Description: referrence
//
//////////////////////////////////////////////////////////////////////

#ifndef CRTP_H
#define CRTP_H 1

#include <string>

namespace supervisor
{

// Polymorphism
class Base
{
private:
    virtual std::string template_method() const = 0;

public:
    virtual ~Base() = default;

    virtual std::string foo() const
    {
        return "Base::foo";
    }

    std::string something() const
    {
        return template_method();
    }
};

class Derived : public Base
{
private:
    virtual std::string template_method() const
    {
        return "Derived::template_method";
    }

    virtual std::string foo() const
    {
        return "Derived::foo";
    }
};


template <class Derived>
class BaseCRTP
{
public:
    std::string base_foo() const
    {
        return static_cast<const Derived&>(*this).foo();
    }

    std::string something() const
    {
        return static_cast<const Derived&>(*this).template_method();
    }
};

class DerivedCRTP : public BaseCRTP<DerivedCRTP>
{
public:
    std::string foo() const {
        return "DerivedCRTP::foo";
    }

    std::string template_method() const {
        return "DerivedCRTP::template_method";
    }
};


// Without Barton-Nackman trick
class Hoge {
    int _id;
public:
    explicit Hoge(int id) : _id(id) {}

    friend bool operator==(const Hoge& lhs, const Hoge& rhs) {
        return lhs._id == rhs._id;
    }
};

// With Barton-Nackman trick
template <class T>
class equal_comparable {
    friend bool operator!=(const T& lhs, const T& rhs) {
        return !lhs.operator==(rhs);
    }
};

template < class T >
class less_than_comparable {
    friend bool operator>(const T& lhs, const T& rhs) {
        return rhs.operator<(lhs);
    }

    friend bool operator<=(const T& lhs, const T& rhs) {
        return lhs.operator<(rhs) || lhs.operator==(rhs);
    }

    friend bool operator>=(const T& lhs, const T& rhs) {
        return !lhs.operator<(rhs);
    }
};

template <class T>
class Ord : equal_comparable<T>, less_than_comparable<T>
{};


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


#include <memory>

template <class T>
std::string print_share(const std::shared_ptr<T>& p)
{
    return p->print();
}

template <class T>
class enable_shared_from_this
{
private:
    std::weak_ptr<T> _weak_this;

public:
    std::shared_ptr<T> shared_from_this()
    {
        return std::shared_ptr<T>(_weak_this);
    }

    void setup_shared_from_this(const std::shared_ptr<T>& p)
    {
        if (_weak_this.expired())
        {
            _weak_this = p;
        }
    }
};

class Shared : public enable_shared_from_this<Shared>
{
public:
    std::string f()
    {
        return print_share(this->shared_from_this());
    }

    std::string print()
    {
        return "Shared";
    }
};
} // namespace supervisor


#endif // CRTP_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
