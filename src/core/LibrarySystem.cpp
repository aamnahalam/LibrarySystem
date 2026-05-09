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
#include "../services/Review.h"
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

    // Optionally save data
    saveData();
}

// Register new admin (only SuperAdmins can do this)
bool LibrarySystem::registerAdmin(string firstName, string lastName, string email, string password, string level)
{
    // Note: currentAdmin is only set during login and won't be available if called from admin.cpp
    // Better to check the admin object directly in Admin::createAdmin()
    // For now, we'll just validate the level and proceed
    if (level != "SuperAdmin" && level != "Admin") {
        cout << "Error: Invalid access level. Must be 'SuperAdmin' or 'Admin'." << endl;
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
            return true;
        }
    }
    
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
    userFile << "ID | Name | Email | Password | Balance | Membership | LoyaltyPoints | LastBorrowDate | BorrowsToday | LastBorrowMonth | BorrowsThisMonth" << endl;
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
                 << u->getLoyaltyPoints() << " | "
                 << u->getLastBorrowDate() << " | "
                 << u->getBorrowsToday() << " | "
                 << u->getLastBorrowMonth() << " | "
                 << u->getBorrowsThisMonth() << endl;
    }
    userFile.close();

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

    // Save Admins
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

    // Save Reviews
    ofstream reviewFile("reviews.txt");
    if (!reviewFile.is_open())
    {
        cout << "Error: Cannot open reviews.txt" << endl;
        return;
    }
    reviewFile << "ResourceID | UserID | Rating | ReviewText" << endl;
    for (auto &r : resources)
    {
        for (const auto &review : r->getReviews())
        {
            if (review && review->getUser())
            {
                // Replace newlines and pipes with spaces to avoid parsing issues
                string reviewText = review->getReviewText();
                for (char &c : reviewText)
                {
                    if (c == '\n' || c == '|') c = ' ';
                }
                reviewFile << r->getResourceID() << " | "
                           << review->getUser()->getID() << " | "
                           << review->getRatingValue() << " | "
                           << reviewText << endl;
            }
        }
    }
    reviewFile.close();
}

// Load data 
void LibrarySystem::loadData() {
    // Clear existing data before loading
    for (auto user : users) delete user;
    users.clear();
    for (auto resource : resources) delete resource;
    resources.clear();
    for (auto admin : admins) delete admin;
    admins.clear();
    
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
            int loyaltyPoints = stoi(parts[6]);
            
            // Load borrowing limits if available (new fields)
            string lastBorrowDate = (parts.size() > 7) ? parts[7] : "";
            int borrowsToday = (parts.size() > 8) ? stoi(parts[8]) : 0;
            string lastBorrowMonth = (parts.size() > 9) ? parts[9] : "";
            int borrowsThisMonth = (parts.size() > 10) ? stoi(parts[10]) : 0;

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
            
            // Restore borrowing limits
            user->lastBorrowDate = lastBorrowDate;
            user->borrowsToday = borrowsToday;
            user->lastBorrowMonth = lastBorrowMonth;
            user->borrowsThisMonth = borrowsThisMonth;
            
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

    if (admins.empty()) {
        cout << "No admins found. Seeding default admin accounts." << endl;
        admins.push_back(new Admin(101, "Mr", "Boss", "admin@library.com", "admin123", "SuperAdmin"));
        admins.push_back(new Admin(102, "Ms", "Manager", "manager@library.com", "manager123", "Admin"));
        saveData();
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

    // Load Reviews
    ifstream reviewFile("reviews.txt");
    if (!reviewFile.is_open())
    {
        cout << "No saved review data found. Starting fresh." << endl;
    }
    else
    {
        string line;
        bool isFirstLine = true;
        while (getline(reviewFile, line))
        {
            if (isFirstLine && line.find("ResourceID |") == 0)
            {
                isFirstLine = false;
                continue;
            }
            if (line.empty()) continue;
            
            // Parse the line
            stringstream ss(line);
            string token;
            vector<string> parts;
            while (getline(ss, token, '|'))
            {
                size_t start = token.find_first_not_of(" \t");
                size_t end = token.find_last_not_of(" \t");
                if (start != string::npos && end != string::npos)
                {
                    token = token.substr(start, end - start + 1);
                }
                parts.push_back(token);
            }
            
            if (parts.size() < 4) continue;
            
            int resourceID = stoi(parts[0]);
            int userID = stoi(parts[1]);
            int rating = stoi(parts[2]);
            string reviewText = parts[3];
            
            // Find the resource
            Resource* resource = nullptr;
            for (auto& res : resources)
            {
                if (res->getResourceID() == resourceID)
                {
                    resource = res;
                    break;
                }
            }
            
            // Find the user
            User* user = nullptr;
            for (auto& u : users)
            {
                if (u->getID() == userID)
                {
                    user = u;
                    break;
                }
            }
            
            // Create and add review if both resource and user exist
            if (resource && user)
            {
                Review* review = new Review(rating, reviewText, user);
                resource->addReview(review);
            }
        }
        reviewFile.close();
        cout << "Reviews loaded from reviews.txt" << endl;
    }
}