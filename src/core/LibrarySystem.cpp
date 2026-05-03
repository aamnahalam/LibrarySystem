#include "LibrarySystem.h"
#include "../users/user.h"
#include "../users/admin.h"
#include "../resources/Resource.h"
#include "../transactions/BorrowRecord.h"
#include "../users/admin.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

LibrarySystem::LibrarySystem()
{
    currentUser = nullptr;
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
}

void LibrarySystem::addAdmin(Admin *a)
{
    admins.push_back(a);
    cout << "Admin added: " << a->getFullName() << endl;
}

// Authenticate user
bool LibrarySystem::authenticate(string email, string password)
{
    for (auto &user : users)
    {
        if (user->getEmail() == email && user->getPassword() == password)
        {
            currentUser = user;
            cout << "Login Successful. Welcome, " << user->getFullName() << endl;
            return true;
        }
    }
    cout << "Login Failed. Invalid email or password." << endl;
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
    for (auto &u : users)
    {
        userFile << u->getID() << "|" << u->getFullName() << "|" << u->getEmail() << "|" << u->getPassword() << "|" << u->getAccountBalance() << "\n";
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
    for (auto &r : resources)
    {
        resFile << r->getResourceID() << "|"
                << r->getTitle() << "|"
                << r->getCategory() << "|"
                << (r->getAvailability() ? "1" : "0") << "|"
                << r->getRating() << "|"
                << r->getBorrowCount() << "\n";
    }
    resFile.close();
    cout << "Resources saved to resources.txt" << endl;

    // Save Borrow History
    ofstream histFile("borrow_history.txt");
    if (!histFile.is_open())
    {
        cout << "Error: Cannot open borrow_history.txt" << endl;
        return;
    }
    for (auto &u : users)
    {
        for (const auto &record : u->getBorrowHistory())
        {
            histFile << u->getID() << "|"
                     << record.getResourceName() << "|"
                     << record.getBorrowDate() << "|"
                     << record.getDueDate() << "|"
                     << (record.getReturnStatus() ? "1" : "0") << "\n";
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
        return;
    }
    string line;
    while (getline(userFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);
        if (parts.size() < 5) continue;
        int    id      = stoi(parts[0]);
        string name    = parts[1];
        string email   = parts[2];
        string pass    = parts[3];
        double balance = stod(parts[4]);
        string firstName = name, lastName = "";
        size_t sp = name.find(' ');
        if (sp != string::npos) { firstName = name.substr(0, sp); lastName = name.substr(sp + 1); }
        users.push_back(new User(id, firstName, lastName, email, pass, balance));
    }
    userFile.close();
    cout << "User data loaded from users.txt" << endl;
}