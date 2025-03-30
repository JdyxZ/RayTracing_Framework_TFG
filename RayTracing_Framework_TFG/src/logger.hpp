#pragma once

// Headers
#include "core.hpp"

enum log_message_type
{
    _ERROR,
    _WARNING,
    _INFO
};

struct Logger
{
public:
    
    Logger();

    static string error(string location, string description);
    static string warn(string location, string description);
    static string info(string location, string description);
    static vector<string> messages();
    static void clear();

private:

    static vector<string> _messages;

    static string new_message(log_message_type type, string& location, string& description);
};



