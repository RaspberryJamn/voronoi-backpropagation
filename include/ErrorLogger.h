#ifndef ERRORLOGGER_H
#define ERRORLOGGER_H
#include <iostream>

class ErrorLogger {
    public:
        ErrorLogger();
        ~ErrorLogger();

        void PotentialLog(std::string message);
        void PotentialLog(std::string message1, std::string message2);

        void EnableLogging();
        void DisableLogging();

    private:
        bool logging_enabled;
        int log_limit_counter;
        int log_limit_max;
};

#endif // ERRORLOGGER_H
