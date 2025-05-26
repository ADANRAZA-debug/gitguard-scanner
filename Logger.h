#pragma once
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;

class Logger {
public:
    static const string SYSTEM_LOG;
    static const string SECURITY_LOG;
    static const string COMPLAINTS_LOG;
    static const string STATUS_LOG;
    
    static void log(const string& message);
    static void logFailedAttempt(const string& username);
    static void logComplaint(const string& userId, const string& complaintId);
    static void logStatusChange(const string& complaintId, const string& newStatus);
};