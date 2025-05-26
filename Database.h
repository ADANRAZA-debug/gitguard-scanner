#pragma once
#include <string>
#include <vector>
using namespace std;

class Database {
public:
    static bool saveUser(const string& cnic, const string& mobile,
                        const string& fullName, const string& fatherName,
                        const string& address, const string& campus,
                        const string& email, const string& password);
    
    static bool userExists(const string& cnic, const string& email);
    static bool validateUser(const string& cnic, const string& password);
    static bool validateAdmin(const string& adminId, const string& password);
    static bool saveComplaint(const string& userId, const string& category,
                             const string& subCategory, const string& description,
                             const string& complaintId);
    
    static vector<vector<string>> getAllUsers();
    static vector<vector<string>> getAllComplaints();
    static vector<vector<string>> getUserComplaints(const string& userId);
    static bool updateComplaintStatus(const string& complaintId, bool resolved);
};