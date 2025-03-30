// Headers
#include "core.hpp"
#include "logger.hpp"
#include "utilities.hpp"

Logger::Logger() {}

string Logger::error(string location, string description)
{
    string message = new_message(_ERROR, location, description);
    std::cerr << message;
    return message;
}

string Logger::warn(string location, string description)
{
    string message = new_message(_WARNING, location, description);
    std::clog << message;
    return message;
}

string Logger::info(string location, string description)
{
    string message = new_message(_INFO, location, description);
    std::cout << message;
    return message;
}

vector<string> Logger::messages()
{
    return _messages;
}

void Logger::clear()
{
    _messages.clear();
}

string Logger::new_message(log_message_type type, string& location, string& description)
{
    string timestamp = get_current_timestamp("%d/%m/%Y %X", true);

    std::ostringstream message;

    message << "[" << timestamp << "] ";
    switch (type)
    {
    case _ERROR:
        message << "[ERROR] ";
        break;
    case _WARNING:
        message << "[WARNING] ";
        break;
    case _INFO:
        message << "[INFO] ";
        break;
    }
    message << "[" << location << "] ";
    message << "-> \"" << description << "\" \n";

    _messages.push_back(message.str());
    return message.str();
}

// Static members
vector<string> Logger::_messages;

