#pragma once

class interval 
{
public:
    double min, max;
    static const interval empty, universe, unitary;

    interval(); // Default interval is empty
    interval(double min, double max);
    interval(const interval& a, const interval& b); // Creates the interval tightly enclosing the two input intervals.

    double size() const;
    bool is_empty() const;
    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;
    interval expand(double delta) const;
};

// Operator overlaods
interval operator+(const interval& ival, double displacement);
interval operator+(double displacement, const interval& ival);
interval operator*(const interval& ival, double displacement);
interval operator*(double displacement, const interval& ival);



