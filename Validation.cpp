#include "Validation.h"
#include <map>
using namespace std;

bool Validation::isValidCnic(const string& cnic) {
    return regex_match(cnic, regex("^[0-9]{13}$"));
}

bool Validation::isValidMobile(const string& mobile) {
    return regex_match(mobile, regex("^03[0-9]{9}$"));
}

bool Validation::isValidName(const string& name) {
    return regex_match(name, regex("^[a-zA-Z ]{3,50}$"));
}

bool Validation::isValidStudentEmail(const string& email) {
    return regex_match(email, regex("^[0-9]{6}@Students\\.au\\.edu\\.pk$"));
}

bool Validation::isValidAdminPassword(const string& password) {
    return regex_match(password, 
        regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{10,}$"));
}

bool Validation::isValidPassword(const string& password) {
    return regex_match(password, 
        regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{8,12}$"));
}

bool Validation::isValidCampus(const string& campus) {
    return (campus == "1" || campus == "2" || campus == "3");
}

bool Validation::isValidComplaintText(const string& text) {
    return text.length() > 0 && text.length() <= 250;
}

bool Validation::isValidCategory(const string& category) {
    return category >= "1" && category <= "6";
}

bool Validation::isValidSubCategory(const string& category, const string& subCategory) {
    static const map<string, vector<string>> validSubs = {
        {"1", {"1", "2", "3", "4"}},
        {"2", {"1", "2", "3", "4"}},
        {"3", {"1", "2", "3", "4"}},
        {"4", {"1", "2", "3", "4"}},
        {"5", {"1", "2", "3", "4"}},
        {"6", {"1", "2", "3", "4"}}
    };
    
    auto it = validSubs.find(category);
    if (it != validSubs.end()) {
        for (const auto& validSub : it->second) {
            if (subCategory == validSub) {
                return true;
            }
        }
    }
    return false;
}