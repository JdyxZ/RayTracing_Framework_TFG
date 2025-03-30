﻿// Internal Headers
#include "core.hpp"
#include "logger.hpp"
#include "utilities.hpp"

// External Headers
#include "../external/vibrant_console.hpp"

void LogMessage::print()
{
    switch (type)
    {
    case(_ERROR):
        std::cerr << timestamp << " " << dye::red("[ERROR]") << " " << location << " " << "-> " << description << std::endl;
        break;
    case(_WARNING):
        std::clog << timestamp << " " << dye::yellow("[WARNING]") << " " << location << " " << "-> " << description << std::endl;
        break;
    case(_INFO):
        std::cout << timestamp << " " << dye::green("[INFO]") << " " << location << " " << "-> " << description << std::endl;
        break;
    }
}

string LogMessage::str()
{
    switch (type)
    {
    case(_ERROR):
        return timestamp + " [ERROR] " + location + " " + "-> " + description + "\n";
    case(_WARNING):
        return timestamp + " [WARNING] " + location + " " + "-> " + description + "\n";
    case(_INFO):
        return timestamp + " [INFO] " + location + " " + "-> " + description + "\n";
    }

    return "Error in parsing LogMessage to string";
}

Logger::Logger() {}

string Logger::error(string location, string description)
{
    LogMessage message = new_message(_ERROR, location, description);
    message.print();
    return message.str();
}

string Logger::warn(string location, string description)
{
    LogMessage message = new_message(_WARNING, location, description);
    message.print();
    return message.str();
}

string Logger::info(string location, string description)
{
    LogMessage message = new_message(_INFO, location, description);
    message.print();
    return message.str();
}

vector<LogMessage> Logger::messages()
{
    return _messages;
}

void Logger::clear()
{
    _messages.clear();
}

LogMessage Logger::new_message(log_message_type type, string& location, string& description)
{
    LogMessage message;

    string timestamp = get_current_timestamp("%d/%m/%Y %X", true);

    message.timestamp = "[" + timestamp + "]";
    message.type = type;
    message.location = "[" + location + "]";
    message.description = "\"" + description + "\"";

    _messages.push_back(message);
    return message;
}

// Static members
vector<LogMessage> Logger::_messages;

