#include "User.h"
#include "Database.h"
#include "Validation.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

User::User(const string& cnic, const string& mobile, 
         const string& fullName, const string& fatherName,
         const string& address, const string& campus,
         const string& email, const string& password) :
    cnic(cnic), mobile(mobile), fullName(fullName), fatherName(fatherName),
    address(address), campus(campus), email(email), password(password) {}

string User::getCnic() const { return cnic; }
string User::getEmail() const { return email; }
string User::getPassword() const { return password; }

bool User::saveToDatabase() const {
    return Database::saveUser(cnic, mobile, fullName, fatherName, 
                            address, campus, email, password);
}

bool User::getValidInput(const string& prompt, string& output, 
                        const function<bool(const string&)>& validator,
                        const string& errorMsg, int maxAttempts) {
    for (int attempts = 0; attempts < maxAttempts; attempts++) {
        cout << prompt << " (or '0' to cancel): ";
        getline(cin, output);
        if (output == "0") return false;
        if (validator(output)) return true;
        cout << errorMsg << "\n";
    }
    cout << "Too many invalid attempts. Returning to menu.\n";
    return false;
}

bool User::isCnicApproved(const string& cnic) {
    ifstream file("valid_cnics.dat");
    string line;
    while (getline(file, line)) {
        if (line == cnic) return true;
    }
    return false;
}

bool User::isEmailApproved(const string& email) {
    ifstream file("valid_emails.dat");
    string line;
    while (getline(file, line)) {
        if (line == email) return true;
    }
    return false;
}

bool User::signIn(string& outCnic) {
    string cnic, password;
    cout << "\nUser Login\n";
    cout << "CNIC (or '0' to cancel): ";
    getline(cin, cnic);
    
    if (cnic == "0") return false;
    
    if (!Validation::isValidCnic(cnic)) {
        cout << "Invalid CNIC format!\n";
        return false;
    }
    
    cout << "Password: ";
    getline(cin, password);
    
    if (Database::validateUser(cnic, password)) {
        outCnic = cnic;
        cout << "Login successful!\n";
        return true;
    }
    cout << "Invalid CNIC or password!\n";
    return false;
}

bool User::signUp() {
    string cnic, mobile, fullName, fatherName, address, email, password, confirmPassword;
    string campus;
    const int maxAttempts = 3;
    
    cout << "\nUser Sign Up (enter '0' at any time to cancel)\n";

    // CNIC validation
    if (!getValidInput("CNIC (13 digits)", cnic, [](const string& input) {
        return Validation::isValidCnic(input);
    }, "Invalid CNIC format!", maxAttempts)) return false;

    if (!isCnicApproved(cnic)) {
        cout << "CNIC not approved. Contact admin.\n";
        return false;
    }
    if (isUserExists(cnic, "")) {
        cout << "CNIC already registered!\n";
        return false;
    }

    // Email validation
    if (!getValidInput("Email (6digits@Students.au.edu.pk)", email, [](const string& input) {
        return Validation::isValidStudentEmail(input);
    }, "Invalid email format!", maxAttempts)) return false;

    if (!isEmailApproved(email)) {
        cout << "Email not approved. Contact admin.\n";
        return false;
    }
    if (isUserExists("", email)) {
        cout << "Email already registered!\n";
        return false;
    }

    // Mobile validation
    if (!getValidInput("Mobile (03XXXXXXXXX)", mobile, [](const string& input) {
        return Validation::isValidMobile(input);
    }, "Invalid mobile format!", maxAttempts)) return false;

    // Name validations
    if (!getValidInput("Full Name", fullName, [](const string& input) {
        return Validation::isValidName(input);
    }, "Invalid name format!", maxAttempts)) return false;

    if (!getValidInput("Father's Name", fatherName, [](const string& input) {
        return Validation::isValidName(input);
    }, "Invalid name format!", maxAttempts)) return false;

    // Address
    cout << "Address (or '0' to cancel): ";
    getline(cin, address);
    if (address == "0") return false;

    // Campus selection
    int campusChoice = 0;
    for (int attempts = 0; attempts < maxAttempts; attempts++) {
        cout << "Select Campus (1-3 or 0 to cancel):\n";
        cout << "1. Kamra Campus\n2. Multan Campus\n3. Islamabad Campus\n";
        cout << "Choice: ";
        getline(cin, campus);
        if (campus == "0") return false;
        if (Validation::isValidCampus(campus)) {
            campusChoice = stoi(campus);
            break;
        }
        cout << "Invalid campus selection!\n";
        if (attempts == maxAttempts - 1) return false;
    }

    string campusName;
    switch(campusChoice) {
        case 1: campusName = "Kamra Campus"; break;
        case 2: campusName = "Multan Campus"; break;
        case 3: campusName = "Islamabad Campus"; break;
    }

    // Password validation
    if (!getValidInput("Password (8-12 chars with special chars)", password, [](const string& input) {
        return Validation::isValidPassword(input);
    }, "Invalid password format!", maxAttempts)) return false;

    do {
        cout << "Confirm Password (or '0' to cancel): ";
        getline(cin, confirmPassword);
        if (confirmPassword == "0") return false;
        if (password != confirmPassword) {
            cout << "Passwords don't match!\n";
        }
    } while (password != confirmPassword);

    User newUser(cnic, mobile, fullName, fatherName, address, campusName, email, password);
    if (newUser.saveToDatabase()) {
        cout << "Registration successful!\n";
        return true;
    }
    cout << "Registration failed!\n";
    return false;
}

bool User::isUserExists(const string& cnic, const string& email) {
    return Database::userExists(cnic, email);
}

bool User::verifyEmailDomain(const string& email) {
    return email.find("@Students.au.edu.pk") != string::npos;
}