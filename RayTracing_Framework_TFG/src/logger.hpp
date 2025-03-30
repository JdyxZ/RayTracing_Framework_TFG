﻿#pragma once

// Headers
#include "core.hpp"

enum log_message_type
{
    _ERROR,
    _WARNING,
    _INFO
};

struct LogMessage
{
    string timestamp;
    log_message_type type;
    string location;
    string description;

    void print();
    string str();
};

struct Logger
{
public:
    
    Logger();

    static string error(string location, string description);
    static string warn(string location, string description);
    static string info(string location, string description);
    static vector<LogMessage> messages();
    static void clear();

private:

    static vector<LogMessage> _messages;

    static LogMessage new_message(log_message_type type, string& location, string& description);
};



