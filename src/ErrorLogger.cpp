#include "ErrorLogger.h"

ErrorLogger::ErrorLogger() {
    this->log_limit_counter = 0;
    this->log_limit_max = 100;
    this->logging_enabled = false;
}

ErrorLogger::~ErrorLogger() {
    this->log_limit_counter = 0;
    this->log_limit_max = 100;
    this->logging_enabled = false;
}

void ErrorLogger::PotentialLog(std::string message) {
    if (!logging_enabled) {
        return;
    }
    if (this->log_limit_counter < this->log_limit_max) {
        this->log_limit_counter++;
        std::cout << message << std::endl;
    }
}

void ErrorLogger::PotentialLog(std::string message1, std::string message2) {
    if (!logging_enabled) {
        return;
    }
    if (this->log_limit_counter < this->log_limit_max) {
        this->log_limit_counter++;
        std::cout << message1 << message2;
    }
}

void ErrorLogger::EnableLogging() {
    this->logging_enabled = true;
}

void ErrorLogger::DisableLogging() {
    this->logging_enabled = false;
}
