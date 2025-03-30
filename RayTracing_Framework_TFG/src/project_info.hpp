#pragma once

// Headers
#include "core.hpp"

struct ProjectInfo
{
public:
    static const string version;
    static const string build_configuration;
    static const string compiler;

private:
    static string getBuildConfiguration();
    static string getCompilerInfo();
};
