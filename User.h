#pragma once
#include <string>
#include <vector>
#include <functional> // Add this line
using namespace std;

class User {
private:
    string cnic;
    string mobile;
    string fullName;
    string fatherName;
    string address;
    string campus;
    string email;
    string password;
    
    static bool getValidInput(const string& prompt, string& output, 
                            const function<bool(const string&)>& validator,
                            const string& errorMsg, int maxAttempts);
    
public:
    User(const string& cnic, const string& mobile, 
         const string& fullName, const string& fatherName,
         const string& address, const string& campus,
         const string& email, const string& password);
    
    string getCnic() const;
    string getEmail() const;
    string getPassword() const;
    
    bool saveToDatabase() const;
    
    static bool signUp();
    static bool signIn(string& outCnic);
    static bool verifyEmailDomain(const string& email);
    static bool isUserExists(const string& cnic, const string& email);
    static bool isCnicApproved(const string& cnic);
    static bool isEmailApproved(const string& email);
};