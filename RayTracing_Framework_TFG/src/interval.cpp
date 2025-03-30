// Headers 
#include "core.hpp"
#include "interval.hpp"

interval::interval() 
{
    min = +infinity;
    max = -infinity;
}

interval::interval(double min, double max)
{
    if (min > max)
        std::swap(min, max);

    this->min = min;
    this->max = max;
}

interval::interval(const interval& a, const interval& b)
{
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

double interval::size() const
{
    return max - min;
}

bool interval::is_empty() const
{
    return size() > 0.0;
}

bool interval::contains(double x) const
{
    return min <= x && x <= max;
}

bool interval::surrounds(double x) const
{
    return min < x && x < max;
}

double interval::clamp(double x) const
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

interval interval::expand(double delta) const
{
    auto padding = delta / 2;
    return interval(min - padding, max + padding);
}

interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}

interval operator*(const interval& ival, double displacement) {
    return interval(ival.min * displacement, ival.max * displacement);
}

interval operator*(double displacement, const interval& ival) {
    return ival * displacement;
}

// Static members
const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
const interval interval::unitary = interval(0.0, 1.0);