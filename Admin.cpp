#include "Admin.h"
#include "Database.h"
#include "Validation.h"
#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
using namespace std;

Admin::Admin(const string& id, const string& pwd) : adminId(id), password(pwd) {}

bool Admin::authenticate() {
    string id, pwd;
    cout << "\nAdmin Login\nID: ";
    getline(cin, id);
    cout << "Password: ";
    getline(cin, pwd);
    
    return Database::validateAdmin(id, pwd);
}

void Admin::showAdminMenu() const {
    cout << "\nAdmin Dashboard\n";
    cout << "1. View All Users\n";
    cout << "2. View All Complaints\n";
    cout << "3. Manage Approved Emails\n";
    cout << "4. Manage Approved CNICs\n";
    cout << "5. Logout\n";
}

void Admin::viewAllUsers() const {
    vector<vector<string>> users = Database::getAllUsers();
    cout << "\nAll Registered Users:\n";
    cout << "-----------------------------------------------------------------\n";
    cout << left << setw(15) << "CNIC" << setw(25) << "Name" 
         << setw(25) << "Email" << setw(15) << "Campus\n";
    cout << "-----------------------------------------------------------------\n";
    
    for (const auto& user : users) {
        cout << left << setw(15) << user[0] << setw(25) << user[2] 
             << setw(25) << user[6] << setw(15) << user[5] << "\n";
    }
}

void Admin::viewAllComplaints() const {
    vector<vector<string>> complaints = Database::getAllComplaints();
    cout << "\nAll Complaints:\n";
    cout << "-----------------------------------------------------------------\n";
    cout << left << setw(12) << "ComplaintID" << setw(15) << "User CNIC" 
         << setw(20) << "Category" << setw(10) << "Status\n";
    cout << "-----------------------------------------------------------------\n";
    
    for (const auto& comp : complaints) {
        try {
            if (comp.size() > 6) {
                cout << left << setw(12) << comp[5] << setw(15) << comp[0] 
                     << setw(20) << getCategoryName(comp[1])
                     << setw(10) << (comp[6] == "1" ? "Resolved" : "Pending") << "\n";
            }
        } catch (...) {
            cout << left << setw(12) << (comp.size() > 5 ? comp[5] : "N/A")
                 << setw(15) << (comp.size() > 0 ? comp[0] : "N/A")
                 << setw(20) << "Unknown"
                 << setw(10) << (comp.size() > 6 ? (comp[6] == "1" ? "Resolved" : "Pending") : "N/A") << "\n";
        }
    }
    
    cout << "\nEnter Complaint ID to update status (0 to cancel): ";
    string compId;
    getline(cin, compId);
    
    if (compId != "0") {
        cout << "1. Mark as Resolved\n2. Mark as Pending\nChoice: ";
        string choice;
        getline(cin, choice);
        
        bool newStatus = (choice == "1");
        if (Database::updateComplaintStatus(compId, newStatus)) {
            cout << "Status updated successfully!\n";
            Logger::logStatusChange(compId, newStatus ? "Resolved" : "Pending");
        } else {
            cout << "Failed to update status!\n";
        }
    }
}

void Admin::addStudentEmail(const string& email) const {
    ofstream file("valid_emails.dat", ios::app);
    if (file) {
        file << email << "\n";
        cout << "Email added to approved list.\n";
    } else {
        cout << "Error saving email!\n";
    }
}

void Admin::removeStudentEmail(const string& email) const {
    ifstream in("valid_emails.dat");
    ofstream out("temp.dat");
    string line;
    bool found = false;
    
    while (getline(in, line)) {
        if (line != email) out << line << "\n";
        else found = true;
    }
    
    in.close();
    out.close();
    
    remove("valid_emails.dat");
    rename("temp.dat", "valid_emails.dat");
    
    if (found) cout << "Email removed from approved list.\n";
    else cout << "Email not found in approved list.\n";
}

void Admin::addStudentCnic(const string& cnic) const {
    ofstream file("valid_cnics.dat", ios::app);
    if (file) {
        file << cnic << "\n";
        cout << "CNIC added to approved list.\n";
    } else {
        cout << "Error saving CNIC!\n";
    }
}

void Admin::removeStudentCnic(const string& cnic) const {
    ifstream in("valid_cnics.dat");
    ofstream out("temp.dat");
    string line;
    bool found = false;
    
    while (getline(in, line)) {
        if (line != cnic) out << line << "\n";
        else found = true;
    }
    
    in.close();
    out.close();
    
    remove("valid_cnics.dat");
    rename("temp.dat", "valid_cnics.dat");
    
    if (found) cout << "CNIC removed from approved list.\n";
    else cout << "CNIC not found in approved list.\n";
}

string Admin::getCategoryName(const string& categoryId) const {
    static const map<string, string> categories = {
        {"1", "Course-Related"}, {"2", "Instructor"}, {"3", "Facilities"},
        {"4", "Administrative"}, {"5", "Financial"}, {"6", "Other"}
    };
    auto it = categories.find(categoryId);
    return it != categories.end() ? it->second : "Unknown";
}

string Admin::getSubCategoryName(const string& categoryId, const string& subCategoryId) const {
    static const map<string, map<string, string>> subCategories = {
        {"1", {{"1", "Content"}, {"2", "Curriculum"}, {"3", "Materials"}, {"4", "Objectives"}}},
        {"2", {{"1", "Behavior"}, {"2", "Teaching"}, {"3", "Knowledge"}, {"4", "Grading"}}},
        {"3", {{"1", "Classroom"}, {"2", "Library"}, {"3", "Lab"}, {"4", "Sanitation"}}},
        {"4", {{"1", "Admission"}, {"2", "Documents"}, {"3", "Certificate"}, {"4", "Registration"}}},
        {"5", {{"1", "Fees"}, {"2", "Payment"}, {"3", "Refund"}, {"4", "Financial Aid"}}},
        {"6", {{"1", "Security"}, {"2", "Transport"}, {"3", "Cafeteria"}, {"4", "General"}}}
    };
    
    auto catIt = subCategories.find(categoryId);
    if (catIt != subCategories.end()) {
        auto subIt = catIt->second.find(subCategoryId);
        if (subIt != catIt->second.end()) {
            return subIt->second;
        }
    }
    return "Unknown";
}