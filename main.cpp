#include <iostream>
#include <vector>
#include <string>
#include "User.h"
#include "Admin.h"
#include "Complaint.h"
#include "Logger.h"
#include "Database.h"
#include "Validation.h"
using namespace std;

void showMainMenu() {
    cout << "\nEducational Institution Complaint Management System\n";
    cout << "1. Admin Login\n";
    cout << "2. User Login\n";
    cout << "3. Don't have an account? Sign Up\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void handleAdminSession() {
    Admin admin("admin", "");
    int choice;
    string input;
    
    do {
        admin.showAdminMenu();
        cout << "Enter your choice (1-5): ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                admin.viewAllUsers();
                break;
            case 2:
                admin.viewAllComplaints();
                break;
            case 3: {
                int emailChoice;
                do {
                    cout << "\nManage Approved Emails\n";
                    cout << "1. Add Email\n2. Remove Email\n3. Back\nChoice: ";
                    cin >> emailChoice;
                    cin.ignore();
                    
                    if (emailChoice == 1) {
                        string email;
                        cout << "Enter student email to add (format: 123456@Students.au.edu.pk): ";
                        getline(cin, email);
                        if (Validation::isValidStudentEmail(email)) {
                            admin.addStudentEmail(email);
                            cout << "Email added successfully!\n";
                        } else {
                            cout << "Invalid email format!\n";
                        }
                    } else if (emailChoice == 2) {
                        string email;
                        cout << "Enter student email to remove: ";
                        getline(cin, email);
                        admin.removeStudentEmail(email);
                    } else if (emailChoice != 3) {
                        cout << "Invalid choice!\n";
                    }
                } while (emailChoice != 3);
                break;
            }
            case 4: {
                int cnicChoice;
                do {
                    cout << "\nManage Approved CNICs\n";
                    cout << "1. Add CNIC\n2. Remove CNIC\n3. Back\nChoice: ";
                    cin >> cnicChoice;
                    cin.ignore();
                    
                    if (cnicChoice == 1) {
                        string cnic;
                        cout << "Enter student CNIC to add (13 digits): ";
                        getline(cin, cnic);
                        if (Validation::isValidCnic(cnic)) {
                            admin.addStudentCnic(cnic);
                            cout << "CNIC added successfully!\n";
                        } else {
                            cout << "Invalid CNIC format!\n";
                        }
                    } else if (cnicChoice == 2) {
                        string cnic;
                        cout << "Enter student CNIC to remove: ";
                        getline(cin, cnic);
                        admin.removeStudentCnic(cnic);
                    } else if (cnicChoice != 3) {
                        cout << "Invalid choice!\n";
                    }
                } while (cnicChoice != 3);
                break;
            }
            case 5:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        
        if (choice != 5) {
            cout << "\nPress Enter to continue...";
            getline(cin, input);
        }
    } while (choice != 5);
}

void handleUserSession(const string& cnic) {
    int choice;
    string input;
    
    do {
        cout << "\nUser Dashboard\n";
        cout << "1. Register New Complaint\n";
        cout << "2. View My Complaints\n";
        cout << "3. View Complaint Status\n";
        cout << "4. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: {
                Complaint::showComplaintCategories();
                string category, subCategory, description;
                
                cout << "Enter category number: ";
                getline(cin, category);
                if (!Validation::isValidCategory(category)) {
                    cout << "Invalid category selection!\n";
                    break;
                }
                
                Complaint::showSubCategories(category);
                cout << "Enter subcategory number: ";
                getline(cin, subCategory);
                if (!Validation::isValidSubCategory(category, subCategory)) {
                    cout << "Invalid subcategory selection!\n";
                    break;
                }
                
                cout << "Enter complaint description (max 250 chars): ";
                getline(cin, description);
                if (!Validation::isValidComplaintText(description)) {
                    cout << "Description too long or empty! Max 250 characters allowed.\n";
                    break;
                }
                
                Complaint complaint(cnic, category, subCategory, description);
                if (complaint.saveComplaint()) {
                    cout << "Complaint registered successfully! ID: " << complaint.getComplaintId() << "\n";
                    Logger::logComplaint(cnic, complaint.getComplaintId());
                } else {
                    cout << "Failed to register complaint!\n";
                }
                break;
            }
            case 2: {
                vector<vector<string>> complaints = Database::getUserComplaints(cnic);
                if (complaints.empty()) {
                    cout << "You have no complaints registered.\n";
                    break;
                }
                
                cout << "\nYour Complaints:\n";
                cout << "-----------------------------------------------------------------\n";
                cout << left << setw(12) << "ID" << setw(20) << "Category" 
                     << setw(20) << "Subcategory" << setw(10) << "Status\n";
                cout << "-----------------------------------------------------------------\n";
                
                for (const auto& complaint : complaints) {
                    try {
                        if (complaint.size() > 6) {
                            cout << left << setw(12) << complaint[5] 
                                 << setw(20) << Complaint::getCategoryName(complaint[1])
                                 << setw(20) << Complaint::getSubCategoryName(complaint[1], complaint[2])
                                 << setw(10) << (complaint[6] == "1" ? "Resolved" : "Pending") << "\n";
                        }
                    } catch (...) {
                        cout << left << setw(12) << (complaint.size() > 5 ? complaint[5] : "N/A")
                             << setw(20) << "Unknown"
                             << setw(20) << "Unknown"
                             << setw(10) << (complaint.size() > 6 ? (complaint[6] == "1" ? "Resolved" : "Pending") : "N/A") << "\n";
                    }
                }
                break;
            }
            case 3: {
                cout << "Enter Complaint ID: ";
                string compId;
                getline(cin, compId);
                
                vector<vector<string>> complaints = Database::getUserComplaints(cnic);
                bool found = false;
                for (const auto& comp : complaints) {
                    if (comp[5] == compId) {
                        found = true;
                        cout << "\nComplaint Details:\n";
                        cout << "----------------------------------------\n";
                        cout << "ID: " << comp[5] << "\n";
                        cout << "Category: " << Complaint::getCategoryName(comp[1]) << "\n";
                        cout << "Subcategory: " << Complaint::getSubCategoryName(comp[1], comp[2]) << "\n";
                        cout << "Status: " << (comp[6] == "1" ? "Resolved" : "Pending") << "\n";
                        cout << "Date: " << Complaint::formatTimestamp(stol(comp[4])) << "\n";
                        cout << "Description: " << comp[3] << "\n";
                        cout << "----------------------------------------\n";
                        break;
                    }
                }
                if (!found) {
                    cout << "Complaint not found or doesn't belong to you!\n";
                }
                break;
            }
            case 4:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
        
        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            getline(cin, input);
        }
    } while (choice != 4);
}

int main() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1:
                if (Admin::authenticate()) {
                    Logger::log("Admin logged in");
                    handleAdminSession();
                } else {
                    Logger::logFailedAttempt("Admin");
                    cout << "Admin login failed!\n";
                }
                break;
            case 2: {
                string cnic;
                if (User::signIn(cnic)) {
                    Logger::log("User logged in: " + cnic);
                    handleUserSession(cnic);
                } else {
                    Logger::logFailedAttempt("User");
                }
                break;
            }
            case 3:
                if (User::signUp()) {
                    Logger::log("New user registered");
                }
                break;
            case 4:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);
    
    return 0;
}