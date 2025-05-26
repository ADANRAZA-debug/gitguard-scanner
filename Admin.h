#pragma once
#include <string>
#include <vector>
#include "Logger.h"
using namespace std;

class Admin {
private:
    string adminId;
    string password;
    
public:
    Admin(const string& id, const string& pwd);
    
    static bool authenticate();
    void showAdminMenu() const;
    
    void viewAllUsers() const;
    void viewAllComplaints() const;
    void addStudentEmail(const string& email) const;
    void removeStudentEmail(const string& email) const;
    void addStudentCnic(const string& cnic) const;
    void removeStudentCnic(const string& cnic) const;
    void updateComplaintStatus(const string& complaintId, bool resolved) const;
    
    string getCategoryName(const string& categoryId) const;
    string getSubCategoryName(const string& categoryId, const string& subCategoryId) const;
};