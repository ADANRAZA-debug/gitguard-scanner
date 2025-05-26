#pragma once
#include <string>
#include <regex>
using namespace std;

class Validation {
public:
    static bool isValidCnic(const string& cnic);
    static bool isValidMobile(const string& mobile);
    static bool isValidName(const string& name);
    static bool isValidStudentEmail(const string& email);
    static bool isValidAdminPassword(const string& password);
    static bool isValidPassword(const string& password);
    static bool isValidCampus(const string& campus);
    static bool isValidComplaintText(const string& text);
    static bool isValidCategory(const string& category);
    static bool isValidSubCategory(const string& category, const string& subCategory);
};