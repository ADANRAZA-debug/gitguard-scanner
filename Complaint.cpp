#include "Complaint.h"
#include "Database.h"
#include <iostream>
#include <map>
#include <ctime>
#include <iomanip>
using namespace std;

Complaint::Complaint(const string& userId, const string& category,
              const string& subCategory, const string& description) :
    userId(userId), category(category), subCategory(subCategory),
    description(description), timestamp(time(nullptr)), resolved(false),
    complaintId(generateComplaintId()) {}

string Complaint::generateComplaintId() {
    static int counter = 1000;
    return "COMP-" + to_string(++counter);
}

bool Complaint::saveComplaint() const {
    return Database::saveComplaint(userId, category, subCategory, 
                                 description, complaintId);
}

void Complaint::showComplaintCategories() {
    cout << "\nComplaint Categories:\n";
    cout << "1. Course-Related\n";
    cout << "2. Instructor Issues\n";
    cout << "3. Facilities\n";
    cout << "4. Administrative\n";
    cout << "5. Financial\n";
    cout << "6. Other\n";
}

void Complaint::showSubCategories(const string& category) {
    static const map<string, map<string, string>> subCategories = {
        {"1", {{"1", "Content Quality"}, {"2", "Curriculum"}, {"3", "Materials"}, {"4", "Objectives"}}},
        {"2", {{"1", "Behavior"}, {"2", "Teaching"}, {"3", "Knowledge"}, {"4", "Grading"}}},
        {"3", {{"1", "Classroom"}, {"2", "Library"}, {"3", "Lab"}, {"4", "Sanitation"}}},
        {"4", {{"1", "Admission"}, {"2", "Documents"}, {"3", "Certificate"}, {"4", "Registration"}}},
        {"5", {{"1", "Fees"}, {"2", "Payment"}, {"3", "Refund"}, {"4", "Financial Aid"}}},
        {"6", {{"1", "Security"}, {"2", "Transport"}, {"3", "Cafeteria"}, {"4", "General"}}}
    };
    
    auto it = subCategories.find(category);
    if (it != subCategories.end()) {
        cout << "\nSubcategories:\n";
        for (const auto& sub : it->second) {
            cout << sub.first << ". " << sub.second << "\n";
        }
    }
}

string Complaint::getCategoryName(const string& categoryId) {
    static const map<string, string> categories = {
        {"1", "Course-Related"}, {"2", "Instructor Issues"}, {"3", "Facilities"},
        {"4", "Administrative"}, {"5", "Financial"}, {"6", "Other"}
    };
    auto it = categories.find(categoryId);
    return it != categories.end() ? it->second : "Unknown";
}

string Complaint::getSubCategoryName(const string& categoryId, const string& subCategoryId) {
    static const map<string, map<string, string>> subCategories = {
        {"1", {{"1", "Content Quality"}, {"2", "Curriculum"}, {"3", "Materials"}, {"4", "Objectives"}}},
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

string Complaint::formatTimestamp(time_t timestamp) {
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&timestamp));
    return string(buffer);
}