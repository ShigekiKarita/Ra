// concept.hpp
//
// last-edit-by: karita
//
// Description: referrence
//
//////////////////////////////////////////////////////////////////////

#ifndef CONCEPT_H
#define CONCEPT_H 1

#include <cmath>

namespace supervisor
{

    class Point
    {
    private:
        double _x = 0;
        double _y = 0;

    public:
        Point() = default;

        Point(double x, double y) : _x(x), _y(y) {}

        double x() const { return _x; }
        double y() const { return _y; }

        friend Point operator-(const Point& lhs, const Point& rhs)
        {
            return Point(lhs.x() - rhs.x(), lhs.y() - rhs.y());
        }
    };

    template <class P>
    double _distance(const P& lhs, const P& rhs)
    {
        const P d = lhs - rhs;
        return std::sqrt(d.x() * d.x() + d.y() * d.y());
    }

    template <class T>
    struct point_traits
    {
        static double x(const T& p) { return p.x(); }
        static double y(const T& p) { return p.y(); }
    };

    template <class P>
    P subtract(const P& lhs, const P& rhs)
    {
        typedef point_traits<P> traits;
        return P(traits::x(lhs) - traits::x(rhs),
                 traits::y(lhs) - traits::y(rhs));
    }

    template <class P>
    double distance(const P& lhs, const P& rhs)
    {
        typedef point_traits<P> traits;
        const P d = subtract(lhs, rhs);

        return std::sqrt(traits::x(d) * traits::x(d) +
                         traits::y(d) * traits::y(d));
    }



    class AnotherPoint
    {
    private:
        double _x = 0;
        double _y = 0;

    public:
        AnotherPoint() = default;

        AnotherPoint(double x, double y) : _x(x), _y(y) {}

        double getX() const { return _x; }
        double getY() const { return _y; }

        // NOTE: no operator -
    };

    template <>
    struct point_traits<AnotherPoint>
    {
        static double x(const AnotherPoint& p) { return p.getX(); }
        static double y(const AnotherPoint& p) { return p.getY(); }
    };

    template <class Point>
    class Line
    {
    private:
        Point _p1, _p2;

    public:
        typedef Point point_type;

        Line() = default;
        Line(const Point& p1, const Point& p2)
            : _p1(p1), _p2(p2)
        {}

        const Point& p1() const { return _p1; }
        const Point& p2() const { return _p2; }
    };

} // namespace supervisor

#endif // CONCEPT_H
//////////////////////////////////////////////////////////////////////
// $Log:$
//
