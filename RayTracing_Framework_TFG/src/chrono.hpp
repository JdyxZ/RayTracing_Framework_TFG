﻿#pragma once

// Headers
#include "core.hpp"

// Aliases
using time_point = std::chrono::high_resolution_clock::time_point;
using time_duration = std::chrono::high_resolution_clock::duration;

struct elapsed_time
{
    int hours;
    int minutes;
    int seconds;
    int milliseconds;
};

struct Chrono
{
public:
    Chrono();

    void start();
    void end();
    elapsed_time elapsed() const;
    int elapsed_miliseconds() const;
    string elapsed_to_string() const;

    Chrono& operator+=(const Chrono& c);
    Chrono operator+(const Chrono& c) const;

private:
    optional<time_point> start_time;
    time_duration total_elapsed;
};