#include "LibrarySystem.h"
#include "../users/user.h"
#include "../users/admin.h"
#include "../resources/Resource.h"
#include "../resources/PrimePickBook.h"
#include "../resources/ClassicShelfBook.h"
#include "../resources/BudgetPickBook.h"
#include "../Membership/Membership.h"
#include "../Membership/NormalMembership.h"
#include "../Membership/ExtraMembership.h"
#include "../Membership/DeluxeMembership.h"
#include "../transactions/BorrowRecord.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

static bool parseInt(const string &value, int &out)
{
    try {
        size_t idx;
        out = stoi(value, &idx);
        return idx == value.size();
    } catch (...) {
        return false;
    }
}

static bool parseDouble(const string &value, double &out)
{
    try {
        size_t idx;
        out = stod(value, &idx);
        return idx == value.size();
    } catch (...) {
        return false;
    }
}

LibrarySystem::LibrarySystem()
{
    currentUser = nullptr;
    currentAdmin = nullptr;
    loadData();
}

LibrarySystem::~LibrarySystem()
{
    for (auto user : users)
    {
        delete user;
    }
    for (auto admin : admins)
    {
        delete admin;
    }
    for (auto resource : resources)
    {
        delete resource;
    }
    for (auto record : borrowRecords)
    {
        delete record;
    }
}

void LibrarySystem::addUser(User *u)
{
    users.push_back(u);
    cout << "User added: " << u->getFullName() << endl;
    saveData();
}

void LibrarySystem::addAdmin(Admin *a)
{
    admins.push_back(a);
    cout << "Admin added: " << a->getFullName() << endl;
}

void LibrarySystem::registerUser(string firstName, string lastName, string email, string password, double initialBalance)
{
    // Find the next available ID
    int nextId = 1;
    for (auto &u : users) {
        if (u->getID() >= nextId) {
            nextId = u->getID() + 1;
        }
    }

    // Create new user
    User *newUser = new User(nextId, firstName, lastName, email, password, initialBalance);

    // Set default membership
    newUser->setMembership(new NormalMembership());

    // Add to users
    users.push_back(newUser);

    cout << "User registered successfully: " << newUser->getFullName() << " (ID: " << nextId << ")" << endl;

    // Optionally save data
    saveData();
}

// Register new admin (only SuperAdmins can do this)
bool LibrarySystem::registerAdmin(string firstName, string lastName, string email, string password, string level)
{
    // Check if current user is a SuperAdmin
    if (!currentAdmin || currentAdmin->getAccessLevel() != "SuperAdmin")
    {
        cout << "Error: Only SuperAdmins can create new admins." << endl;
        return false;
    }

    // Check if email already exists
    for (auto &admin : admins) {
        if (admin->getEmail() == email) {
            cout << "Error: Admin with this email already exists." << endl;
            return false;
        }
    }
    for (auto &user : users) {
        if (user->getEmail() == email) {
            cout << "Error: User with this email already exists." << endl;
            return false;
        }
    }

    // Find the next available ID (for admins, use IDs >= 100)
    int nextId = 100;
    for (auto &a : admins) {
        if (a->getID() >= nextId) {
            nextId = a->getID() + 1;
        }
    }

    // Create new admin
    Admin *newAdmin = new Admin(nextId, firstName, lastName, email, password, level);

    // Add to admins
    admins.push_back(newAdmin);

    cout << "Admin registered successfully: " << newAdmin->getFullName() << " (ID: " << nextId << ", Level: " << level << ")" << endl;

    // Save data
    saveData();
    return true;
}

// Authenticate user or admin
bool LibrarySystem::authenticate(string email, string password)
{
    // Try to authenticate as user
    for (auto &user : users)
    {
        if (user->getEmail() == email && user->getPassword() == password)
        {
            currentUser = user;
            currentAdmin = nullptr;  // Clear admin session
            cout << "Login Successful. Welcome, " << user->getFullName() << endl;
            return true;
        }
    }
    
    // Try to authenticate as admin
    for (auto &admin : admins)
    {
        if (admin->getEmail() == email && admin->getPassword() == password)
        {
            currentAdmin = admin;
            currentUser = nullptr;  // Clear user session
            cout << "Admin Login Successful. Welcome, " << admin->getFullName() << " (" << admin->getAccessLevel() << ")" << endl;
            return true;
        }
    }
    
    cout << "Login Failed. Invalid email or password." << endl;
    return false;
}

User *LibrarySystem::getCurrentUser() const
{
    return currentUser;
}

Admin *LibrarySystem::getCurrentAdmin() const
{
    return currentAdmin;
}

Resource *LibrarySystem::getResourceByID(int resourceID) const
{
    for (auto &res : resources)
    {
        if (res->getResourceID() == resourceID)
            return res;
    }
    return nullptr;
}

bool LibrarySystem::borrowResource(int resourceID, string date)
{
    if (!currentUser)
    {
        cout << "Cannot borrow: no user signed in." << endl;
        return false;
    }
    Resource *res = getResourceByID(resourceID);
    if (!res)
    {
        cout << "Cannot borrow: resource not found." << endl;
        return false;
    }
    bool success = currentUser->borrowresources(res, date);
    if (success)
        saveData();
    return success;
}

double LibrarySystem::returnResource(int resourceID, string date)
{
    if (!currentUser)
    {
        cout << "Cannot return: no user signed in." << endl;
        return 0.0;
    }
    Resource *res = getResourceByID(resourceID);
    if (!res)
    {
        cout << "Cannot return: resource not found." << endl;
        return 0.0;
    }
    double result = currentUser->returnresources(res, date);
    if (result >= 0.0)
        saveData();
    return result;
}

bool LibrarySystem::changeUserMembershipTier(int userID, int tier, bool confirm)
{
    for (auto &user : users)
    {
        if (user->getID() == userID)
        {
            bool changed = user->changeMembershipTier(tier, confirm);
            if (changed)
                saveData();
            return changed;
        }
    }
    cout << "User not found with ID " << userID << endl;
    return false;
}

void LibrarySystem::showAllUsers() const
{
    if (users.empty())
    {
        cout << "No users registered." << endl;
        return;
    }
    for (auto &u : users)
        u->displayInfo();
}

void LibrarySystem::showAllAdmins() const
{
    if (admins.empty())
    {
        cout << "No admins registered." << endl;
        return;
    }
    for (auto &a : admins)
        a->displayInfo();
}
// Logout
void LibrarySystem::logout()
{
    if (currentUser)
        cout << currentUser->getFullName() << " logged out." << endl;
    currentUser = nullptr;
}

// Search resource by keyword
vector<Resource *> LibrarySystem::searchResource(string keyword)
{
    vector<Resource *> result;
    for (auto r : resources)
        if (r->getTitle().find(keyword) != string::npos ||
            r->getCategory().find(keyword) != string::npos)
            result.push_back(r);
    return result;
}

// Filter by category
vector<Resource *> LibrarySystem::filterResources(string category)
{
    vector<Resource *> result;

    for (auto r : resources)
    {
        if (r->getCategory() == category)
        {
            result.push_back(r);
        }
    }

    return result;
}

// Filter available resources
vector<Resource *> LibrarySystem::filterByAvailability()
{
    vector<Resource *> result;

    for (auto r : resources)
    {
        if (r->getAvailability())
        {
            result.push_back(r);
        }
    }

    return result;
}

// Filter new arrivals
vector<Resource *> LibrarySystem::filterByNewArrivals()
{
    vector<Resource *> result;

    for (auto r : resources)
    {
        if (r->getIsNewArrival())
        {
            result.push_back(r);
        }
    }

    return result;
}

// Filter most borrowed
vector<Resource *> LibrarySystem::filterByMostBorrowed()
{
    vector<Resource *> result = resources;

    sort(result.begin(), result.end(), [](Resource *a, Resource *b)
         { return a->getBorrowCount() > b->getBorrowCount(); });

    return result;
}

// Filter by rating
vector<Resource *> LibrarySystem::filterByRating()
{
    vector<Resource *> result = resources;

    sort(result.begin(), result.end(), [](Resource *a, Resource *b)
         { return a->getReviewScore() > b->getReviewScore(); });

    return result;
}

// Filter by user preference
vector<Resource *> LibrarySystem::filterByUserPreference(User *u)
{
    vector<Resource *> result;
    string pref = u->getPreferredCategory();
    if (pref.empty())
        return result;
    for (auto r : resources)
        if (r->getCategory() == pref)
            result.push_back(r);
    return result;
}

// FILE HANDLING:

// Save data
void LibrarySystem::saveData()
{
    // Save Users
    ofstream userFile("users.txt");
    if (!userFile.is_open())
    {
        cout << "Error: Cannot open users.txt" << endl;
        return;
    }
    userFile << "ID | Name | Email | Password | Balance | Membership | LoyaltyPoints" << endl;
    for (auto &u : users)
    {
        string membershipType = "Essential";
        if (u->membership)
            membershipType = u->membership->getLevelName();

        userFile << u->getID() << " | "
                 << u->getFullName() << " | "
                 << u->getEmail() << " | "
                 << u->getPassword() << " | "
                 << u->getAccountBalance() << " | "
                 << membershipType << " | "
                 << u->getLoyaltyPoints() << endl;
    }
    userFile.close();
    cout << "Users saved to users.txt" << endl;

    // Save Resources
    ofstream resFile("resources.txt");
    if (!resFile.is_open())
    {
        cout << "Error: Cannot open resources.txt" << endl;
        return;
    }
    resFile << "ID | Type | Title | Author | Category | Available | Rating | BorrowCount" << endl;
    for (auto &r : resources)
    {
        string typeName = "Unknown";
        if (dynamic_cast<PrimePickBook *>(r))
            typeName = "PrimePick";
        else if (dynamic_cast<ClassicShelfBook *>(r))
            typeName = "ClassicShelf";
        else if (dynamic_cast<BudgetPickBook *>(r))
            typeName = "BudgetPick";

        resFile << r->getResourceID() << " | "
                << typeName << " | "
                << r->getTitle() << " | "
                << r->getAuthor() << " | "
                << r->getCategory() << " | "
                << (r->getAvailability() ? "1" : "0") << " | "
                << r->getRating() << " | "
                << r->getBorrowCount() << endl;
    }
    resFile.close();
    cout << "Resources saved to resources.txt" << endl;

    // Save Admins
    cout << "Saving " << admins.size() << " admins..." << endl;
    ofstream adminFile("admins.txt");
    if (!adminFile.is_open())
    {
        cout << "Error: Cannot open admins.txt" << endl;
        return;
    }
    adminFile << "ID | Name | Email | Password | AccessLevel" << endl;
    for (auto &a : admins)
    {
        adminFile << a->getID() << " | "
                  << a->getFullName() << " | "
                  << a->getEmail() << " | "
                  << a->getPassword() << " | "
                  << a->getAccessLevel() << endl;
    }
    adminFile.close();
    cout << "Admins saved to admins.txt" << endl;

    // Save Borrow History
    ofstream histFile("borrow_history.txt");
    if (!histFile.is_open())
    {
        cout << "Error: Cannot open borrow_history.txt" << endl;
        return;
    }
    histFile << "UserID | ResourceID | ResourceName | BorrowDate | DueDate | Returned | ReturnDate" << endl;
    for (auto &u : users)
    {
        for (const auto &record : u->getBorrowHistory())
        {
            histFile << u->getID() << " | "
                     << record.getResourceID() << " | "
                     << record.getResourceName() << " | "
                     << record.getBorrowDate() << " | "
                     << record.getDueDate() << " | "
                     << (record.getReturnStatus() ? "1" : "0") << " | "
                     << record.getReturnDate() << endl;
        }
    }
    histFile.close();
    cout << "Borrow history saved to borrow_history.txt" << endl;
}

// Load data 
void LibrarySystem::loadData() {
    ifstream userFile("users.txt");
    if (!userFile.is_open()) {
        cout << "No saved user data found. Starting fresh." << endl;
    } else {
        string line;
        bool isFirstLine = true;
        while (getline(userFile, line)) {
            if (isFirstLine && line.find("ID |") == 0) {
                isFirstLine = false;
                continue;
            }
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            vector<string> parts;
            while (getline(ss, token, '|')) {
                // Trim spaces around the token
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                if (start != string::npos && end != string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                parts.push_back(token);
            }
            if (parts.size() < 7) continue;

            int id;
            double balance;
            int loyaltyPoints;
            string name    = parts[1];
            string email   = parts[2];
            string pass    = parts[3];
            string membershipType = parts[5];
            if (!parseInt(parts[0], id) || !parseDouble(parts[4], balance) || !parseInt(parts[6], loyaltyPoints))
                continue;

            string firstName = name, lastName = "";
            size_t sp = name.find(' ');
            if (sp != string::npos) { firstName = name.substr(0, sp); lastName = name.substr(sp + 1); }

            User *user = new User(id, firstName, lastName, email, pass, balance);
            if (membershipType == "Deluxe")
                user->setMembership(new DeluxeMembership());
            else if (membershipType == "Extra")
                user->setMembership(new ExtraMembership());
            else
                user->setMembership(new NormalMembership());
            for (int i = 0; i < loyaltyPoints; ++i) user->earnpoints(1);
            users.push_back(user);
        }
    }
    userFile.close();
    cout << "User data loaded from users.txt" << endl;

    // Load Resources
    ifstream resFile("resources.txt");
    if (!resFile.is_open())
    {
        cout << "No saved resource data found. Continuing." << endl;
    }
    else
    {
        string line;
        bool isFirstLine = true;
        while (getline(resFile, line))
        {
            if (isFirstLine && line.find("ID |") == 0) {
                isFirstLine = false;
                continue;
            }
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            vector<string> parts;
            while (getline(ss, token, '|')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                if (start != string::npos && end != string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                parts.push_back(token);
            }
            if (parts.size() < 8) continue;

            int id;
            double rating;
            int borrowCount;
            string type = parts[1];
            string title = parts[2];
            string author = parts[3];
            string category = parts[4];
            bool available = parts[5] == "1";
            if (!parseInt(parts[0], id) || !parseDouble(parts[6], rating) || !parseInt(parts[7], borrowCount))
                continue;

            Resource *res = nullptr;
            if (type == "PrimePick")
                res = new PrimePickBook(id, title, author, category);
            else if (type == "ClassicShelf")
                res = new ClassicShelfBook(id, title, author, category);
            else if (type == "BudgetPick")
                res = new BudgetPickBook(id, title, author, category);
            else
                continue;

            res->isAvailable = available;
            res->rating = rating;
            res->borrowCount = borrowCount;
            resources.push_back(res);
        }
        resFile.close();
        cout << "Resource data loaded from resources.txt" << endl;
    }

    // Load Admins
    ifstream adminFile("admins.txt");
    if (!adminFile.is_open())
    {
        cout << "No saved admin data found. Continuing." << endl;
    }
    else
    {
        string line;
        bool isFirstLine = true;
        while (getline(adminFile, line))
        {
            if (isFirstLine && line.find("ID |") == 0) {
                isFirstLine = false;
                continue;
            }
            if (line.empty()) continue;
            stringstream ss(line);
            string token;
            vector<string> parts;
            while (getline(ss, token, '|')) {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                if (start != string::npos && end != string::npos) {
                    token = token.substr(start, end - start + 1);
                }
                parts.push_back(token);
            }
            if (parts.size() < 5) continue;

            int id;
            string name = parts[1];
            string email = parts[2];
            string password = parts[3];
            string accessLevel = parts[4];
            if (!parseInt(parts[0], id))
                continue;

            string firstName = name, lastName = "";
            size_t sp = name.find(' ');
            if (sp != string::npos) { 
                firstName = name.substr(0, sp); 
                lastName = name.substr(sp + 1); 
            }

            Admin *admin = new Admin(id, firstName, lastName, email, password, accessLevel);
            admins.push_back(admin);
        }
        adminFile.close();
        cout << "Admin data loaded from admins.txt" << endl;
    }

    // Load Borrow History
    ifstream histFile("borrow_history.txt");
    if (!histFile.is_open())
    {
        cout << "No saved borrow history found. Continuing." << endl;
        return;
    }

    string histLine;
    bool isFirstLine = true;
    while (getline(histFile, histLine))
    {
        if (isFirstLine && histLine.find("UserID |") == 0) {
            isFirstLine = false;
            continue;
        }
        if (histLine.empty()) continue;
        stringstream ss(histLine);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) {
            size_t start = token.find_first_not_of(" \t");
            size_t end = token.find_last_not_of(" \t");
            if (start != string::npos && end != string::npos) {
                token = token.substr(start, end - start + 1);
            }
            parts.push_back(token);
        }
        if (parts.size() < 7) continue;

        int userID;
        int resourceID;
        string resourceName = parts[2];
        string borrowDate = parts[3];
        string dueDate = parts[4];
        bool returned = parts[5] == "1";
        string returnDate = parts[6];
        if (!parseInt(parts[0], userID) || !parseInt(parts[1], resourceID))
            continue;

        User *user = nullptr;
        for (auto &u : users)
        {
            if (u->getID() == userID)
            {
                user = u;
                break;
            }
        }
        if (!user) continue;

        BorrowRecord record(user, resourceID, resourceName, borrowDate, dueDate);
        if (returned)
            record.markAsReturned(returnDate);

        user->borrowHistory.push_back(record);

        if (!returned)
        {
            for (auto &res : resources)
            {
                if (res->getResourceID() == resourceID)
                {
                    res->updateAvailability(false);
                    user->borrowedResources.push_back(res);
                    break;
                }
            }
        }
    }
    histFile.close();
    cout << "Borrow history loaded from borrow_history.txt" << endl;
}