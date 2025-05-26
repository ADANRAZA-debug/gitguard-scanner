#include "Logger.h"

const string Logger::SYSTEM_LOG = "system.log";
const string Logger::SECURITY_LOG = "security.log";
const string Logger::COMPLAINTS_LOG = "complaints.log";
const string Logger::STATUS_LOG = "status_changes.log";

void Logger::log(const string& message) {
    ofstream logFile(SYSTEM_LOG, ios::app);
    if (logFile.is_open()) {
        time_t now = time(nullptr);
        logFile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " - " 
               << message << endl;
        logFile.close();
    }
}

void Logger::logFailedAttempt(const string& username) {
    ofstream logFile(SECURITY_LOG, ios::app);
    if (logFile.is_open()) {
        time_t now = time(nullptr);
        logFile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " - "
               << "Failed login attempt for: " << username << endl;
        logFile.close();
    }
}

void Logger::logComplaint(const string& userId, const string& complaintId) {
    ofstream logFile(COMPLAINTS_LOG, ios::app);
    if (logFile.is_open()) {
        time_t now = time(nullptr);
        logFile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " - "
               << "User " << userId << " registered complaint " << complaintId << endl;
        logFile.close();
    }
}

void Logger::logStatusChange(const string& complaintId, const string& newStatus) {
    ofstream logFile(STATUS_LOG, ios::app);
    if (logFile.is_open()) {
        time_t now = time(nullptr);
        logFile << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S") << " - "
               << "Complaint " << complaintId << " status changed to: " << newStatus << endl;
        logFile.close();
    }
}