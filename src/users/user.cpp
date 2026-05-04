#include "user.h"
#include "../resources/Resource.h"
#include "../transactions/BorrowRecord.h"
#include "../Membership/Membership.h"
#include "../Membership/NormalMembership.h"
#include "../Membership/ExtraMembership.h"
#include "../Membership/DeluxeMembership.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int convertDate(string date);
int addDays(int date, int days);
string formatDate(int date);

static string getTierName(int tier);
static int getMembershipIndex(const string &level);
static double getTierCost(int tier);

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

    int borrowDateInt = convertDate(date);
    if (borrowDateInt == 0)
    {
        cout << "Invalid Borrow Date." << endl;
        return false;
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

    string dueDateStr = formatDate(addDays(borrowDateInt, 7));

    cout << "Borrowing: " << r->getTitle() << " | Borrow: " << date << " | Due: " << dueDateStr << endl;
    BorrowRecord record(this, r->getResourceID(), r->getTitle(), date, dueDateStr);
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
                int lateDays = returnDate - dueDate;
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

string User::getMembershipChangeNotice(int tier) const
{
    string targetName = getTierName(tier);
    if (targetName == "Unknown")
        return "Invalid membership choice. Choose 1, 2 or 3.";

    int currentTier = 1;
    string currentName = "Essential";
    if (membership)
    {
        currentName = membership->getLevelName();
        currentTier = getMembershipIndex(currentName);
    }

    if (currentTier == tier)
        return "You already have the " + targetName + " membership.";

    if (tier < currentTier)
    {
        return "Warning: You are downgrading from " + currentName + " to " + targetName + ". "
               "This will reduce your borrow limit and benefits. Confirm before switching.";
    }

    double cost = getTierCost(tier);
    string costText = (cost > 0.0) ? "Rs." + to_string((int)cost) : "Free";
    return "You are switching from " + currentName + " to " + targetName + ". "
           "This change costs " + costText + ". Confirm before switching.";
}

bool User::changeMembershipTier(int tier, bool confirm)
{
    double cost = 0.0;
    string tierName;
    switch (tier)
    {
        case 1:
            cost = 0.0;
            tierName = "Essential";
            break;
        case 2:
            cost = 10.0;
            tierName = "Extra";
            break;
        case 3:
            cost = 20.0;
            tierName = "Deluxe";
            break;
        default:
            cout << "Invalid membership choice. Choose 1, 2 or 3." << endl;
            return false;
    }

    if (membership != nullptr && membership->getLevelName() == tierName)
    {
        cout << "You already have the " << tierName << " membership." << endl;
        return false;
    }

    if (tier < getMembershipIndex(membership ? membership->getLevelName() : string("Essential")) && !confirm)
    {
        cout << "Warning: You are about to downgrade to " << tierName << ". "
             << "This will lower your borrow limit and reduce benefits. "
             << "Please confirm the downgrade to proceed." << endl;
        return false;
    }

    if (accountbalance < cost)
    {
        cout << "Insufficient balance to choose the " << tierName << " plan. "
             << "Required: Rs." << cost << ", Available: Rs." << accountbalance << endl;
        return false;
    }

    Membership* newMembership = nullptr;
    switch (tier)
    {
        case 1:
            newMembership = new NormalMembership();
            break;
        case 2:
            newMembership = new ExtraMembership();
            break;
        case 3:
            newMembership = new DeluxeMembership();
            break;
    }

    if (cost > 0.0)
    {
        accountbalance -= cost;
        cout << "Rs." << cost << " deducted for " << tierName << " membership." << endl;
        cout << "New balance: Rs." << accountbalance << endl;
    }
    else
    {
        cout << "Selected Essential (Free) tier." << endl;
    }

    setMembership(newMembership);
    cout << "Membership changed successfully." << endl;
    return true;
}

void User::showMembershipOptions() const
{
    cout << "\nMembership Options:" << endl;
    cout << "  1. Essential (Free) - borrow up to 2 books, no fine discount" << endl;
    cout << "  2. Extra ($10/month) - borrow up to 5 books, 25% fine discount, priority reservation" << endl;
    cout << "  3. Deluxe ($20/month) - borrow up to 10 books, 50% fine discount, free waiver, priority queue" << endl;
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

int User::getLoyaltyPoints() const
{
    return loyaltypoints;
}

// Redeem loyalty points for discount on balance
bool User::redeemPointsForDiscount(int pointsToRedeem)
{
    if (loyaltypoints >= pointsToRedeem)
    {
        loyaltypoints -= pointsToRedeem;
        double discount = (pointsToRedeem / 100.0) * 50; // 100 points = Rs.50 discount
        accountbalance += discount;
        cout << "Redeemed " << pointsToRedeem << " loyalty points for Rs." << discount << " balance credit!" << endl;
        return true;
    }
    cout << "Insufficient loyalty points! You have: " << loyaltypoints << " points" << endl;
    return false;
}

// Redeem loyalty points for fine waiver pass
bool User::redeemPointsForFineFreePass()
{
    if (loyaltypoints >= 100)
    {
        loyaltypoints -= 100;
        cout << "Redeemed 100 loyalty points for ONE fine-free pass!" << endl;
        cout << "  Your next overdue fine will be waived!" << endl;
        return true;
    }
    cout << "Insufficient loyalty points for fine waiver! Need: 100, Have: " << loyaltypoints << endl;
    return false;
}

void User::displayLoyaltySummary() const
{
    cout << "\n" << string(50, '=') << endl;
    cout << "        LOYALTY REWARDS SUMMARY" << endl;
    cout << string(50, '=') << endl;
    cout << "Current Points: " << loyaltypoints << endl;
    cout << "Equivalent Discount: Rs." << (loyaltypoints / 100.0 * 50) << endl;
    cout << "\nRedemption Options:" << endl;
    cout << "  - 100 points -> Fine-free pass (next overdue book)" << endl;
    cout << "  - 200 points -> Rs.100 balance credit" << endl;
    cout << "  - 500 points -> Rs.250 balance credit + free book rental" << endl;
    cout << "  - 1000 points -> Membership upgrade!" << endl;
    cout << string(50, '=') << endl;
}

void User::checkAndUpgradeMembership()
{
    if (membership == nullptr) return;

    string currentTier = membership->getLevelName();

    if (loyaltypoints >= 1000 && currentTier != "Deluxe")
    {
        cout << "\nCONGRATULATIONS! You've earned enough loyalty points for a Deluxe upgrade!" << endl;
        setMembership(new DeluxeMembership());
        loyaltypoints -= 500;
        cout << "   Deluxe membership activated!" << endl;
    }
    else if (loyaltypoints >= 500 && currentTier == "Essential")
    {
        cout << "\nYou're eligible for an Extra membership upgrade!" << endl;
        setMembership(new ExtraMembership());
        loyaltypoints -= 200;
        cout << "   Extra membership activated!" << endl;
    }
    else
    {
        cout << "\nNo automatic membership upgrade available at this time." << endl;
    }
}

void User::displayMembershipDetails() const
{
    if (membership != nullptr)
    {
        membership->displayDetails();
    }
}

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

static string getTierName(int tier)
{
    switch (tier)
    {
        case 1:
            return "Essential";
        case 2:
            return "Extra";
        case 3:
            return "Deluxe";
        default:
            return "Unknown";
    }
}

static int getMembershipIndex(const string &level)
{
    if (level == "Extra")
        return 2;
    if (level == "Deluxe")
        return 3;
    return 1;
}

static double getTierCost(int tier)
{
    switch (tier)
    {
        case 2:
            return 10.0;
        case 3:
            return 20.0;
        default:
            return 0.0;
    }
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
