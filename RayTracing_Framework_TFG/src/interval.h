#ifndef INTERVAL_H
#define INTERVAL_H

class interval {
public:
    double min, max;

    interval() // Default interval is empty
    {
        min = +infinity;
        max = -infinity;
    } 

    interval(double min, double max)
    {
        if (min > max)
            std::swap(min, max);

        this->min = min;
        this->max = max;
    }

    interval(const interval& a, const interval& b) // Create the interval tightly enclosing the two input intervals.
    {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    double size() const 
    {
        return max - min;
    }

    bool is_empty() const
    {
        return size() > 0.0;
    }

    bool contains(double x) const 
    {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const 
    {
        return min < x && x < max;
    }

    double clamp(double x) const 
    {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    interval expand(double delta) const 
    {
        auto padding = delta / 2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe, unitary;
};


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

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
const interval interval::unitary = interval(0.0, 1.0);

#endif
