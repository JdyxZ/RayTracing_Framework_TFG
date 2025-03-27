#ifndef LOGGER_H
#define LOGGER_H

// Forward declaration to avoid circular dependency
inline string get_current_timestamp(const char* fmt, bool milliseconds);

enum log_message_type
{
    _ERROR,
    _WARNING,
    _INFO
};

struct Logger
{
public:
    
    Logger() {}

    static string error(string location, string description)
    {
        string message = new_message(_ERROR, location, description);
        std::cerr << message;
        return message;
    }

    static string warn(string location, string description)
    {
        string message = new_message(_WARNING, location, description);
        std::clog << message;
        return message;
    }

    static string info(string location, string description)
    {
        string message = new_message(_INFO, location, description);
        std::cout << message;
        return message;
    }

    static vector<string> messages()
    {
        return _messages;
    }

    static void clear()
    {
        _messages.clear();
    }

private:

    static vector<string> _messages;

    static string new_message(log_message_type type, string& location, string& description)
    {
        string timestamp = get_current_timestamp("%x %X", true);

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
};

// Add this in logger.cpp or similar implementation file
vector<string> Logger::_messages;

#endif

