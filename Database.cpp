#include "Database.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <limits>
using namespace std;

bool Database::saveUser(const string& cnic, const string& mobile,
                      const string& fullName, const string& fatherName,
                      const string& address, const string& campus,
                      const string& email, const string& password) {
    ofstream outFile("users.dat", ios::app);
    if (outFile.is_open()) {
        outFile << cnic << "," << mobile << "," << fullName << ","
               << fatherName << "," << address << "," << campus << ","
               << email << "," << password << "\n";
        outFile.close();
        return true;
    }
    return false;
}

bool Database::userExists(const string& cnic, const string& email) {
    ifstream inFile("users.dat");
    if (!inFile) return false;
    
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string storedCnic, storedEmail;
        
        getline(ss, storedCnic, ',');
        for (int i = 0; i < 5; i++) ss.ignore(numeric_limits<streamsize>::max(), ',');
        getline(ss, storedEmail, ',');
        
        if (storedCnic == cnic || storedEmail == email) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

bool Database::validateUser(const string& cnic, const string& password) {
    ifstream inFile("users.dat");
    if (!inFile) return false;
    
    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string storedCnic, storedPwd;
        
        getline(ss, storedCnic, ',');
        for (int i = 0; i < 6; i++) ss.ignore(numeric_limits<streamsize>::max(), ',');
        getline(ss, storedPwd);
        
        if (storedCnic == cnic && storedPwd == password) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

bool Database::validateAdmin(const string& adminId, const string& password) {
    ifstream inFile("admin_credentials.dat");
    if (!inFile) {
        ofstream outFile("admin_credentials.dat");
        outFile << "admin,Admin@12345\n";
        outFile.close();
        inFile.open("admin_credentials.dat");
    }

    string line;
    while (getline(inFile, line)) {
        size_t commaPos = line.find(',');
        if (commaPos != string::npos) {
            string storedId = line.substr(0, commaPos);
            string storedPwd = line.substr(commaPos + 1);
            storedPwd.erase(storedPwd.find_last_not_of("\n\r") + 1);
            
            if (storedId == adminId && storedPwd == password) {
                inFile.close();
                return true;
            }
        }
    }
    inFile.close();
    return false;
}

bool Database::saveComplaint(const string& userId, const string& category,
                           const string& subCategory, const string& description,
                           const string& complaintId) {
    ofstream outFile("complaints.dat", ios::app);
    if (outFile.is_open()) {
        time_t now = time(nullptr);
        outFile << userId << "," << category << "," << subCategory << ","
               << description << "," << now << "," << complaintId << ",0\n";
        outFile.close();
        return true;
    }
    return false;
}

vector<vector<string>> Database::getAllUsers() {
    vector<vector<string>> users;
    ifstream inFile("users.dat");
    string line;
    
    while (getline(inFile, line)) {
        vector<string> userData;
        stringstream ss(line);
        string field;
        
        while (getline(ss, field, ',')) {
            userData.push_back(field);
        }
        users.push_back(userData);
    }
    inFile.close();
    return users;
}

vector<vector<string>> Database::getAllComplaints() {
    vector<vector<string>> complaints;
    ifstream inFile("complaints.dat");
    string line;
    
    while (getline(inFile, line)) {
        vector<string> complaintData;
        stringstream ss(line);
        string field;
        
        while (getline(ss, field, ',')) {
            complaintData.push_back(field);
        }
        complaints.push_back(complaintData);
    }
    inFile.close();
    return complaints;
}

vector<vector<string>> Database::getUserComplaints(const string& userId) {
    vector<vector<string>> userComplaints;
    vector<vector<string>> allComplaints = getAllComplaints();
    
    for (const auto& complaint : allComplaints) {
        if (complaint[0] == userId) {
            userComplaints.push_back(complaint);
        }
    }
    return userComplaints;
}

bool Database::updateComplaintStatus(const string& complaintId, bool resolved) {
    ifstream inFile("complaints.dat");
    ofstream tempFile("temp_complaints.dat");
    bool updated = false;
    
    if (inFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inFile, line)) {
            vector<string> fields;
            stringstream ss(line);
            string field;
            
            while (getline(ss, field, ',')) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 7 && fields[5] == complaintId) {
                fields[6] = resolved ? "1" : "0";
                updated = true;
            }
            
            for (size_t i = 0; i < fields.size(); i++) {
                tempFile << fields[i];
                if (i < fields.size() - 1) tempFile << ",";
            }
            tempFile << "\n";
        }
        
        inFile.close();
        tempFile.close();
        
        if (updated) {
            remove("complaints.dat");
            rename("temp_complaints.dat", "complaints.dat");
        }
    }
    return updated;
}