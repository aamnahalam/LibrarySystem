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
#include "../transactions/Reservation.h"
#include "../services/Review.h"
#include "../exceptions/LibraryException.h"
#include "../exceptions/BorrowLimitExceededException.h"
#include "../exceptions/InsufficientBalanceException.h"
#include "../exceptions/ResourceNotAvailableException.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <climits>
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
    // Validate inputs
    if (firstName.empty() || lastName.empty() || email.empty() || password.empty())
    {
        throw LibraryException("Cannot register: All fields (name, email, password) are required.");
    }
    
    if (password.length() < 6)
    {
        throw LibraryException("Cannot register: Password must be at least 6 characters long.");
    }
    
    if (initialBalance < 0)
    {
        throw InsufficientBalanceException("Cannot register: Initial balance cannot be negative.");
    }
    
    // Check if email already exists
    for (auto &admin : admins) {
        if (admin->getEmail() == email) {
            throw LibraryException("Cannot register: Email already registered as an admin account.");
        }
    }
    for (auto &user : users) {
        if (user->getEmail() == email) {
            throw LibraryException("Cannot register: Email already registered as a user account.");
        }
    }
    
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
    // Validate inputs
    if (firstName.empty() || lastName.empty() || email.empty() || password.empty())
    {
        throw LibraryException("Cannot register admin: All fields (name, email, password) are required.");
    }
    
    if (password.length() < 6)
    {
        throw LibraryException("Cannot register admin: Password must be at least 6 characters long.");
    }
    
    // Note: currentAdmin is only set during login and won't be available if called from admin.cpp
    // Better to check the admin object directly in Admin::createAdmin()
    // For now, we'll just validate the level and proceed
    if (level != "SuperAdmin" && level != "Admin") {
        throw LibraryException("Error: Invalid access level. Must be 'SuperAdmin' or 'Admin'.");
    }

    // Check if email already exists
    for (auto &admin : admins) {
        if (admin->getEmail() == email) {
            throw LibraryException("Cannot register admin: Email already registered as an admin account.");
        }
    }
    for (auto &user : users) {
        if (user->getEmail() == email) {
            throw LibraryException("Cannot register admin: Email already registered as a user account.");
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
    if (email.empty() || password.empty())
    {
        throw LibraryException("Cannot authenticate: Email and password are required.");
    }
    
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
    
    throw LibraryException("Cannot authenticate: Invalid email or password. Please try again or register a new account.");
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
        throw LibraryException("Cannot borrow: No user is currently logged in.");
    }
    Resource *res = getResourceByID(resourceID);
    if (!res)
    {
        throw LibraryException("Cannot borrow: Resource with ID " + to_string(resourceID) + " not found.");
    }
    currentUser->borrowresources(res, date);
    saveData();
    return true;
}

double LibrarySystem::returnResource(int resourceID, string date)
{
    if (!currentUser)
    {
        throw LibraryException("Cannot return: No user is currently logged in.");
    }
    Resource *res = getResourceByID(resourceID);
    if (!res)
    {
        throw LibraryException("Cannot return: Resource with ID " + to_string(resourceID) + " not found.");
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
            user->changeMembershipTier(tier, confirm);
            saveData();
            return true;
        }
    }
    throw LibraryException("Cannot change membership: User with ID " + to_string(userID) + " not found.");
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

    // Save Reservations
    ofstream reservationFile("reservations.txt");
    if (!reservationFile.is_open())
    {
        cout << "Error: Cannot open reservations.txt" << endl;
        return;
    }
    reservationFile << "UserID | ResourceID | ResourceName | ReservationDate | QueuePosition | Status" << endl;
    for (auto &reservation : reservations)
    {
        reservationFile << reservation->getUserID() << " | "
                       << reservation->getResourceID() << " | "
                       << reservation->getResourceName() << " | "
                       << reservation->getReservationDate() << " | "
                       << reservation->getQueuePosition() << " | "
                       << reservation->getStatus() << endl;
    }
    reservationFile.close();
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
            string name    = parts[1];
            string email   = parts[2];
            string pass    = parts[3];
            string membershipType = parts[5];
            int loyaltyPoints = stoi(parts[6]);
            
            // Parse ID and Balance from the file
            id = stoi(parts[0]);
            balance = stod(parts[4]);
            
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

    // Seed initial books if none exist
    if (resources.empty()) {
        cout << "No resources found. Seeding default library books." << endl;
        resources.push_back(new PrimePickBook(5001, "Harry Potter", "J.K. Rowling", "Fantasy"));
        resources.push_back(new PrimePickBook(5002, "The Hobbit", "J.R.R. Tolkien", "Fantasy"));
        resources.push_back(new PrimePickBook(5003, "Pride and Prejudice", "Jane Austen", "Romance"));
        resources.push_back(new ClassicShelfBook(5004, "1984", "George Orwell", "Dystopian"));
        resources.push_back(new ClassicShelfBook(5005, "To Kill a Mockingbird", "Harper Lee", "Fiction"));
        resources.push_back(new ClassicShelfBook(5006, "The Great Gatsby", "F. Scott Fitzgerald", "Fiction"));
        resources.push_back(new BudgetPickBook(5007, "Atomic Habits", "James Clear", "SelfHelp"));
        resources.push_back(new BudgetPickBook(5008, "Sapiens", "Yuval Noah Harari", "History"));
        resources.push_back(new BudgetPickBook(5009, "Peer E Kamil", "Umera Ahmad", "Fiction"));
        resources.push_back(new PrimePickBook(5010, "The Midnight Library", "Matt Haig", "Fantasy"));
        saveData();
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

    // Load Reservations
    ifstream reservationFile("reservations.txt");
    if (!reservationFile.is_open())
    {
        cout << "No saved reservation data found. Starting fresh." << endl;
    }
    else
    {
        string line;
        bool isFirstLine = true;
        while (getline(reservationFile, line))
        {
            if (isFirstLine && line.find("UserID |") == 0)
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
            
            if (parts.size() < 6) continue;
            
            int userID = stoi(parts[0]);
            int resourceID = stoi(parts[1]);
            string resourceName = parts[2];
            string reservationDate = parts[3];
            int queuePosition = stoi(parts[4]);
            string status = parts[5];
            
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
            
            // Create and add reservation if user exists
            if (user)
            {
                Reservation* reservation = new Reservation(user, resourceID, resourceName, reservationDate, queuePosition);
                reservation->setStatus(status);
                reservations.push_back(reservation);
            }
        }
        reservationFile.close();
        cout << "Reservations loaded from reservations.txt" << endl;
    }
}

// RESERVATION SYSTEM METHODS

bool LibrarySystem::reserveBook(int resourceID, string date)
{
    if (!currentUser)
    {
        throw LibraryException("Cannot reserve: No user is currently logged in.");
    }

    Resource* res = getResourceByID(resourceID);
    if (!res)
    {
        throw LibraryException("Cannot reserve: Resource with ID " + to_string(resourceID) + " not found.");
    }

    // Check if book is available - if so, user should borrow instead of reserve
    if (res->getAvailability())
    {
        throw LibraryException("Cannot reserve: This book is currently available. Please use 'Borrow a Book' instead.");
    }

    // Check if already borrowed by current user
    for (auto& record : currentUser->getBorrowHistory()) {
        if (record.getResourceName() == res->getTitle() && !record.getReturnStatus()) {
            throw LibraryException("Cannot reserve: You have already borrowed this book.");
        }
    }

    // Check if user already has a pending reservation for this book
    for (auto& reservation : reservations) {
        if (reservation->getUserID() == currentUser->getID() && 
            reservation->getResourceID() == resourceID &&
            reservation->getStatus() == "pending") {
            throw LibraryException("Cannot reserve: You already have a pending reservation for this book.");
        }
    }

    // Count existing reservations for this book to determine queue position
    int queuePosition = 1;
    for (auto& reservation : reservations) {
        if (reservation->getResourceID() == resourceID && 
            reservation->getStatus() == "pending") {
            queuePosition++;
        }
    }

    // Create and add reservation
    Reservation* newReservation = new Reservation(currentUser, resourceID, res->getTitle(), date, queuePosition);
    reservations.push_back(newReservation);

    saveData();
    return true;
}

bool LibrarySystem::cancelReservation(int resourceID)
{
    if (!currentUser)
    {
        throw LibraryException("Cannot cancel: No user is currently logged in.");
    }

    for (int i = 0; i < reservations.size(); i++) {
        if (reservations[i]->getUserID() == currentUser->getID() && 
            reservations[i]->getResourceID() == resourceID &&
            reservations[i]->getStatus() == "pending") {
            
            reservations[i]->setStatus("cancelled");
            
            // Recalculate queue positions for all remaining pending reservations
            int newPosition = 1;
            for (auto& res : reservations) {
                if (res->getResourceID() == resourceID && res->getStatus() == "pending") {
                    res->setQueuePosition(newPosition);
                    newPosition++;
                }
            }

            saveData();
            return true;
        }
    }

    throw LibraryException("Cannot cancel: No pending reservation found for this book.");
}

void LibrarySystem::viewReservationsForResource(int resourceID) const
{
    Resource* res = getResourceByID(resourceID);
    if (!res) {
        cout << "Resource not found." << endl;
        return;
    }

    cout << "\n================== RESERVATIONS FOR: " << res->getTitle() << " ==================\n";
    cout << left << setw(10) << "Position" 
         << setw(25) << "User" 
         << setw(15) << "Reserved Date" 
         << setw(12) << "Status" << "\n";
    cout << string(70, '-') << "\n";

    bool hasReservations = false;
    for (auto& reservation : reservations) {
        if (reservation->getResourceID() == resourceID && reservation->getStatus() == "pending") {
            hasReservations = true;
            cout << left << setw(10) << reservation->getQueuePosition()
                 << setw(25) << reservation->getUser()->getFullName().substr(0, 24)
                 << setw(15) << reservation->getReservationDate()
                 << setw(12) << reservation->getStatus() << "\n";
        }
    }

    if (!hasReservations) {
        cout << "No pending reservations for this book.\n";
    }
}

void LibrarySystem::viewAllReservations() const
{
    cout << "\n================== ALL RESERVATIONS ==================\n";
    cout << left << setw(8) << "ID" 
         << setw(25) << "Book" 
         << setw(20) << "User"
         << setw(15) << "Reserved Date"
         << setw(10) << "Queue" 
         << setw(12) << "Status" << "\n";
    cout << string(95, '-') << "\n";

    bool hasReservations = false;
    for (auto& reservation : reservations) {
        if (reservation->getStatus() == "pending") {
            hasReservations = true;
            cout << left << setw(8) << reservation->getResourceID()
                 << setw(25) << reservation->getResourceName().substr(0, 24)
                 << setw(20) << reservation->getUser()->getFullName().substr(0, 19)
                 << setw(15) << reservation->getReservationDate()
                 << setw(10) << reservation->getQueuePosition()
                 << setw(12) << reservation->getStatus() << "\n";
        }
    }

    if (!hasReservations) {
        cout << "No pending reservations in the system.\n";
    }
}

void LibrarySystem::fulfillNextReservation(int resourceID, string date)
{
    // Find the next pending reservation for this resource
    Reservation* nextReservation = nullptr;
    int lowestPosition = INT_MAX;

    for (auto& reservation : reservations) {
        if (reservation->getResourceID() == resourceID && 
            reservation->getStatus() == "pending" &&
            reservation->getQueuePosition() < lowestPosition) {
            lowestPosition = reservation->getQueuePosition();
            nextReservation = reservation;
        }
    }

    if (nextReservation) {
        User* reservationUser = nextReservation->getUser();
        
        // NOTE: We don't check daily borrow limit here because fulfilling a reservation
        // just marks it as "ready to collect" - it doesn't actually borrow the book yet.
        // The actual borrow happens in collectReservedBook() which will check all limits.
        
        // Silently fulfill without showing user names
        nextReservation->setStatus("fulfilled");
        
        // Recalculate queue positions for remaining pending reservations
        int newPosition = 1;
        for (auto& res : reservations) {
            if (res->getResourceID() == resourceID && res->getStatus() == "pending") {
                res->setQueuePosition(newPosition);
                newPosition++;
            }
        }
        
        saveData();
    }
}

bool LibrarySystem::collectReservedBook(int resourceID, string date)
{
    if (!currentUser)
    {
        throw LibraryException("Cannot collect: No user is currently logged in.");
    }

    Resource* res = getResourceByID(resourceID);
    if (!res)
    {
        throw LibraryException("Cannot collect: Resource not found.");
    }

    // Find the fulfilled reservation
    Reservation* collectedRes = nullptr;
    for (auto& reservation : reservations) {
        if (reservation->getUserID() == currentUser->getID() && 
            reservation->getResourceID() == resourceID &&
            reservation->getStatus() == "fulfilled") {
            collectedRes = reservation;
            break;
        }
    }

    if (!collectedRes)
    {
        throw LibraryException("Cannot collect: No fulfilled reservation found for this book.");
    }

    // SECURITY CHECK: Verify user hasn't hit daily borrow limit
    // Use borrowsToday counter (never decrements on return, only resets per calendar day)
    int borrowsToday = currentUser->getBorrowsToday();
    
    // If it's a different day, counter resets to 0
    if (currentUser->getLastBorrowDate() != date) {
        borrowsToday = 0;  // Different day, counter resets
    }

    if (borrowsToday >= 2) {
        throw LibraryException("Cannot collect: You have reached daily borrow limit (2 books per day TOTAL). The limit resets at midnight.");
    }

    // Calculate due date (14 days from today)
    time_t now = time(nullptr);
    time_t dueTime = now + (14 * 24 * 60 * 60);
    struct tm* dueinfo = localtime(&dueTime);
    char dueBuffer[20];
    strftime(dueBuffer, sizeof(dueBuffer), "%Y-%m-%d", dueinfo);
    string dueDate(dueBuffer);

    // Create borrow record (bypass availability check since reservation was fulfilled)
    BorrowRecord record(currentUser, resourceID, res->getTitle(), date, dueDate);
    currentUser->borrowHistory.push_back(record);
    
    // INCREMENT borrowsToday counter (same as borrowresources())
    if (date == currentUser->getLastBorrowDate()) {
        // Same day, increment counter
        currentUser->incrementBorrowsToday();
    } else {
        // New day, reset counter and set date
        currentUser->setLastBorrowDate(date);
        currentUser->setBorrowsToday(1);  // First borrow of the new day
    }
    
    // Mark resource as unavailable
    res->updateAvailability(false);
    currentUser->borrowedResources.push_back(res);

    // Mark reservation as collected
    collectedRes->setStatus("collected");

    saveData();
    return true;
}