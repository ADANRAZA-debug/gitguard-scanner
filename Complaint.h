#pragma once
#include <string>
#include <vector>
#include <ctime>
using namespace std;

class Complaint {
private:
    string userId;
    string category;
    string subCategory;
    string description;
    time_t timestamp;
    bool resolved;
    string complaintId;
    
public:
    Complaint(const string& userId, const string& category,
              const string& subCategory, const string& description);
    
    bool saveComplaint() const;
    static void showComplaintCategories();
    static void showSubCategories(const string& category);
    static string generateComplaintId();
    static string getCategoryName(const string& categoryId);
    static string getSubCategoryName(const string& categoryId, const string& subCategoryId);
    static string formatTimestamp(time_t timestamp);
    
    string getComplaintId() const { return complaintId; }
    string getUserId() const { return userId; }
    string getCategory() const { return category; }
    string getSubCategory() const { return subCategory; }
    string getDescription() const { return description; }
    time_t getTimestamp() const { return timestamp; }
    bool isResolved() const { return resolved; }
};