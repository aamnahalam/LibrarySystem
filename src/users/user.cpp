#include "user.h"
#include "../resources/Resource.h"
#include "../transactions/BorrowRecord.h"
#include "../Membership/Membership.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int convertDate(string date);
int addDays(int date, int days);
string formatDate(int date);

// Constructor
User::User(int id, string firstName, string lastName, string email, string password, double balance)
    : Person(id, firstName, lastName, email, password)
{
    accountbalance = balance;
    loyaltypoints = 0;
    membership = nullptr;
    readingFrequency = 0;
    isLocked = false;
    borrowsToday = 0;
    lastBorrowDate = "";
}
// Destructor
User::~User()
{
    delete membership; // Clean up any dynamically allocated resources if needed
}

void User::lock()
{
    isLocked = true;
    cout << getFullName() << " account locked." << endl;
}
void User::unlock()
{
    isLocked = false;
    cout << getFullName() << " account unlocked." << endl;
}
bool User::getLockStatus() const
{
    return isLocked;
}

// Member functions
bool User::borrowresources(Resource *r, string date)
{
    // Check locked
    if (isLocked)
    {
        cout << "Access Denied: Account is locked." << endl;
        return false;
    }
    if (accountbalance < 0)
    {
        cout << "Access Denied: Unpaid fines (" << accountbalance << "). Please recharge." << endl;
        return false;
    }
    if (r == nullptr)
    {
        cout << "Invalid Resource." << endl;
        return false;
    }
    if (!r->getAvailability())
    {
        cout << "Resource Not Available." << endl;
        return false;
    }

    // 2 books per day limit — applies to ALL users regardless of membership
    if (date == lastBorrowDate)
    {
        if (borrowsToday >= 2)
        {
            cout << "Daily Limit Reached: Cannot borrow more than 2 books in one day." << endl;
            return false;
        }
    }
    else
    {
        lastBorrowDate = date;
        borrowsToday = 0;
    }

    // Membership-based total borrow limit
    if (membership != nullptr)
    {
        int limit = membership->getMaxBorrowLimit();
        if ((int)borrowedResources.size() >= limit)
        {
            cout << "Borrow Limit Exceeded: Your membership allows max " << limit << " books at a time." << endl;
            return false;
        }
    }
    borrowedResources.push_back(r);
    r->updateAvailability(false);
    r->incrementBorrowCount();
    borrowsToday++;

    // Track favourite categories
    string cat = r->getCategory();
    bool found = false;
    for (auto &c : favouriteCategories)
        if (c == cat)
        {
            found = true;
            break;
        }
    if (!found)
        favouriteCategories.push_back(cat);

    int borrowDateInt = convertDate(date);
    if (borrowDateInt == 0)
    {
        cout << "Invalid Borrow Date." << endl;
        borrowedResources.pop_back();
        r->updateAvailability(true);
        return false;
    }
    string dueDateStr = formatDate(addDays(borrowDateInt, 7));

    cout << "Borrowing: " << r->getTitle() << " | Borrow: " << date << " | Due: " << dueDateStr << endl;
    BorrowRecord record(this, r->getTitle(), date, dueDateStr);
    borrowHistory.push_back(record);
    readingFrequency++;
    earnpoints(5);
    cout << "Resource Borrowed Successfully." << endl;
    return true;
}

bool User::deductFromBalance(double amount)
{
    if (accountbalance >= amount)
    {
        accountbalance -= amount;
        return true;
    }
    return false;
}
double User::returnresources(Resource *r, string date)
{
    bool found = false;
    for (int i = 0; i < (int)borrowedResources.size(); i++)
    {
        if (borrowedResources[i] == r)
        {
            borrowedResources.erase(borrowedResources.begin() + i);
            r->updateAvailability(true);
            found = true;
            break;
        }
    }
    if (!found)
    {
        cout << "Resource Not Found In Borrowed List." << endl;
        return 0.0;
    }

    for (int i = 0; i < (int)borrowHistory.size(); i++)
    {
        if (borrowHistory[i].getResourceName() == r->getTitle() && !borrowHistory[i].getReturnStatus())
        {
            int returnDate = convertDate(date);
            if (returnDate == 0)
            {
                cout << "Invalid Return Date." << endl;
                return 0.0;
            }
            int dueDate = convertDate(borrowHistory[i].getDueDate());
            borrowHistory[i].markAsReturned(date);
            if (returnDate > dueDate)
            {
                int lateDays = getTotalDays(returnDate) - getTotalDays(dueDate);
                if (lateDays < 0)
                    lateDays = 0;
                double fine = lateDays * r->getFineRate();
                if (membership != nullptr)
                    fine *= (1.0 - membership->getFineDiscount());
                cout << "Late Return. Days Late: " << lateDays << " | Fine: " << fine << endl;
                accountbalance -= fine;
                cout << "Remaining Balance: " << accountbalance << endl;
                if (accountbalance < 0)
                    cout << "Warning: Account in debt. Please recharge." << endl;
                return fine;
            }
            else
            {
                earnpoints(10);
                cout << "Returned On Time. No Fine." << endl;
            }
            return 0.0;
        }
    }
    cout << "Record Not Found In History." << endl;
    return 0.0;
}
void User::rechargebalance(double amount)
{
    if (amount <= 0)
    {
        cout << "Invalid Amount." << endl;
        return;
    }
    accountbalance += amount;
    cout << "Balance Recharged. New Balance: " << accountbalance << endl;
}

void User::earnpoints(int points)
{
    loyaltypoints += points;
    cout << "Loyalty points earned: " << points << endl;
}
void User::setMembership(Membership *m)
{
    if (membership)
    {
        delete membership;
    }
    membership = m;
}

// Getters
double User::getAccountBalance() const
{
    return accountbalance;
}
int User::getID() const
{
    return id;
}
int getTotalDays(int dateInt)
{
    int year = dateInt / 10000;
    int month = (dateInt / 100) % 100;
    int day = dateInt % 100;

    // 30-day assumption consistently
    return (year * 360) + (month * 30) + day;
}
string User::getEmail() const
{
    return email;
}
string User::getPassword() const
{
    return password;
}
string User::getFullName() const
{
    return firstName + " " + lastName;
}

string User::getPreferredCategory() const
{
    if (!favouriteCategories.empty())
        return favouriteCategories.back();
    return "";
}

const vector<BorrowRecord> &User::getBorrowHistory() const { return borrowHistory; }

// Date conversion and manipulation functions
int convertDate(string date)
{
    if (date.size() != 10)
    {
        cout << " Invalid Date Format. Use YYYY-MM-DD" << endl;
        return 0;
    }

    try
    {
        // stoi will throw invalid_argument if it hits a letter
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        // You can manually throw an error if the numbers are nonsense
        if (month < 1 || month > 12 || day < 1 || day > 31)
        {
            throw out_of_range("Month or Day is logically incorrect");
        }

        return year * 10000 + month * 100 + day;
    }
    catch (const invalid_argument &e)
    {
        cerr << "Data Error: Date contains non-numeric characters: " << e.what() << endl;
        return 0;
    }
    catch (const out_of_range &e)
    {
        cerr << "Data Error: Date values are logically impossible: " << e.what() << endl;
        return 0;
    }
}
int addDays(int date, int days)
{
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    day += days;

    //(30-day month assumption)
    while (day > 30)
    {
        day -= 30;
        month++;
    }

    while (month > 12)
    {
        month -= 12;
        year++;
    }

    return year * 10000 + month * 100 + day;
}
string formatDate(int date)
{
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    string m = (month < 10 ? "0" : "") + to_string(month);
    string d = (day < 10 ? "0" : "") + to_string(day);

    return to_string(year) + "-" + m + "-" + d;
}
// view  history
void User::viewhistory()
{
    if (borrowHistory.empty())
    {
        cout << " No Borrow History. " << endl;
        return;
    }
    cout << " Borrow History: " << endl;
    for (int i = 0; i < (int)borrowHistory.size(); i++)
    {
        borrowHistory[i].showRecord();
    }
}
// update profile
void User::updateprofile(string firstName, string lastName, string email, string password)
{
    this->firstName = firstName;
    this->lastName = lastName;
    this->email = email;
    this->password = password;
    cout << " Profile Updated Successfully. " << endl;
}

void User::displayInfo()
{
    cout << "User Information: " << endl;
    cout << "-----------------------------" << endl;
    cout << "ID: " << id << endl;
    cout << "User Name: " << firstName << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Balance: " << accountbalance << endl;
    cout << "Loyalty Points: " << loyaltypoints << endl;
    cout << "Membership Level: " << (membership ? membership->getLevelName() : "None") << endl;
    cout << "Borrowed Resources: " << borrowedResources.size() << endl;
    cout << "-----------------------------" << endl;

    if (membership != nullptr)
    {
        membership->displayDetails();
    }
}
// operator overload for user comparison
bool User::operator==(const User &other) const
{
    return id == other.id;
}
