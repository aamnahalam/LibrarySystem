#include "user.h"
#include "../resources/Resource.h"
#include "../transactions/BorrowRecord.h"
#include "../Membership/Membership.h"
#include "../Membership/NormalMembership.h"
#include "../Membership/ExtraMembership.h"
#include "../Membership/DeluxeMembership.h"
#include "../exceptions/LibraryException.h"
#include "../exceptions/BorrowLimitExceededException.h"
#include "../exceptions/InsufficientBalanceException.h"
#include "../exceptions/ResourceNotAvailableException.h"
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
    isLocked = false;
    fineWaiverActive = false;
    borrowsToday = 0;
    lastBorrowDate = "";
    lastBorrowMonth = "";
    borrowsThisMonth = 0;
}
// Destructor
User::~User()
{
    delete membership; // Clean up any dynamically allocated resources if needed
}

void User::lock()
{
    isLocked = true;
}
void User::unlock()
{
    isLocked = false;
}
bool User::getLockStatus() const
{
    return isLocked;
}

// Member functions
void User::borrowresources(Resource *r, string date)
{
    // Check locked
    if (isLocked)
    {
        throw BorrowLimitExceededException("Cannot borrow: Your account is locked due to outstanding fines or violations.");
    }
    if (accountbalance < 0)
    {
        throw InsufficientBalanceException("Cannot borrow: Your account balance is negative. Please recharge your account.");
    }
    if (r == nullptr)
    {
        throw LibraryException("Cannot borrow: Invalid resource provided.");
    }
    if (!r->getAvailability())
    {
        throw ResourceNotAvailableException("Cannot borrow: This resource is currently not available.");
    }

    // Extract month from date (YYYY-MM-DD format)
    string currentMonth = date.substr(0, 7);  // Gets YYYY-MM
    
    // Reset monthly counter if it's a new month
    if (currentMonth != lastBorrowMonth)
    {
        lastBorrowMonth = currentMonth;
        borrowsThisMonth = 0;
    }
    
    // Check monthly limits based on membership
    int monthlyLimit = 2;  // Default for Essential
    if (membership != nullptr)
    {
        string tierName = membership->getLevelName();
        if (tierName == "Extra")
            monthlyLimit = 5;  // Extra: 5 books per month
        else if (tierName == "Deluxe")
            monthlyLimit = 10;  // Deluxe: 10 books per month
    }
    
    // Check if monthly limit exceeded
    if (borrowsThisMonth >= monthlyLimit)
    {
        throw BorrowLimitExceededException("Cannot borrow: You have reached your monthly borrow limit of " + to_string(monthlyLimit) + " books for this month.");
    }
    
    // Daily borrow limit: 2 books per day for all memberships
    if (date == lastBorrowDate)
    {
        if (borrowsToday >= 2)
        {
            throw BorrowLimitExceededException("Cannot borrow: You have reached the daily limit of 2 books per day.");
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
            throw BorrowLimitExceededException("Cannot borrow: You have reached your maximum borrow limit of " + to_string(limit) + " books.");
        }
    }

    int borrowDateInt = convertDate(date);
    if (borrowDateInt == 0)
    {
        throw LibraryException("Cannot borrow: Invalid date format provided.");
    }

    borrowedResources.push_back(r);
    r->updateAvailability(false);
    r->incrementBorrowCount();
    borrowsToday++;
    borrowsThisMonth++;

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

    BorrowRecord record(this, r->getResourceID(), r->getTitle(), date, dueDateStr);
    borrowHistory.push_back(record);
    earnpoints(5);
}

double User::returnresources(Resource *r, string date)
{
    if (r == nullptr)
    {
        throw LibraryException("Cannot return: Invalid resource provided.");
    }

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
        throw LibraryException("Cannot return: This resource was not borrowed by you.");
    }

    for (int i = 0; i < (int)borrowHistory.size(); i++)
    {
        if (borrowHistory[i].getResourceName() == r->getTitle() && !borrowHistory[i].getReturnStatus())
        {
            int returnDate = convertDate(date);
            if (returnDate == 0)
            {
                throw LibraryException("Cannot return: Invalid date format provided.");
            }
            int dueDate = convertDate(borrowHistory[i].getDueDate());
            borrowHistory[i].markAsReturned(date);
            if (returnDate > dueDate)
            {
                // Calculate fine using BorrowRecord's method
                double discountMultiplier = (membership != nullptr) ? 
                    (1.0 - membership->getFineDiscount()) : 1.0;
                
                double fine = borrowHistory[i].calculateFine(r->getFineRate(), discountMultiplier);
                
                // Check if fine waiver is active
                if (fineWaiverActive)
                {
                    fineWaiverActive = false;  // Use the waiver
                    return 0.0;  // No fine charged
                }
                
                if (fine > accountbalance)
                {
                    throw InsufficientBalanceException("Cannot complete return: Fine of Rs." + to_string((int)fine) + " exceeds your account balance.");
                }
                
                accountbalance -= fine;
                return fine;
            }
            else
            {
                earnpoints(10);
                return 0.0;
            }
        }
    }
    throw LibraryException("Cannot return: No active borrow record found for this resource.");
}
void User::rechargebalance(double amount)
{
    if (amount <= 0)
    {
        throw LibraryException("Cannot recharge: Balance amount must be greater than zero.");
    }
    accountbalance += amount;
}

void User::earnpoints(int points)
{
    loyaltypoints += points;
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

void User::changeMembershipTier(int tier, bool confirm)
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
            throw LibraryException("Cannot change membership: Invalid tier. Must be 1 (Essential), 2 (Extra), or 3 (Deluxe).");
    }

    if (membership != nullptr && membership->getLevelName() == tierName)
    {
        throw LibraryException("Cannot change membership: You already have the " + tierName + " membership.");
    }

    if (tier < getMembershipIndex(membership ? membership->getLevelName() : string("Essential")) && !confirm)
    {
        throw LibraryException("Cannot downgrade membership: Downgrade requires confirmation.");
    }

    if (accountbalance < cost)
    {
        throw InsufficientBalanceException("Cannot upgrade membership: Your account balance (Rs." + to_string((int)accountbalance) + ") is insufficient. Upgrade cost is Rs." + to_string((int)cost) + ".");
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
    }

    setMembership(newMembership);
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
string User::getMembershipName() const
{
    return membership ? membership->getLevelName() : "Essential";
}

double User::getFineDiscount() const
{
    return membership ? membership->getFineDiscount() : 0.0;
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

const vector<BorrowRecord> &User::getBorrowHistory() const { return borrowHistory; }

int User::getLoyaltyPoints() const
{
    return loyaltypoints;
}

string User::getLastBorrowDate() const
{
    return lastBorrowDate;
}

int User::getBorrowsToday() const
{
    return borrowsToday;
}

string User::getLastBorrowMonth() const
{
    return lastBorrowMonth;
}

int User::getBorrowsThisMonth() const
{
    return borrowsThisMonth;
}

// Setters for borrow counter
void User::setLastBorrowDate(string date)
{
    lastBorrowDate = date;
}

void User::setBorrowsToday(int count)
{
    borrowsToday = count;
}

void User::incrementBorrowsToday()
{
    borrowsToday++;
}

// Redeem loyalty points for discount on balance
bool User::redeemPointsForDiscount(int pointsToRedeem)
{
    if (loyaltypoints >= pointsToRedeem)
    {
        loyaltypoints -= pointsToRedeem;
        double discount = (pointsToRedeem / 100.0) * 50; // 100 points = Rs.50 discount
        accountbalance += discount;
        return true;
    }
    return false;
}

// Redeem loyalty points for fine waiver pass
bool User::redeemPointsForFineFreePass()
{
    if (loyaltypoints >= 100)
    {
        loyaltypoints -= 100;
        fineWaiverActive = true;  // Activate the waiver
        return true;
    }
    return false;
}

bool User::hasFineWaiverActive() const
{
    return fineWaiverActive;
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

bool User::checkAndUpgradeMembership()
{
    if (membership == nullptr) return false;

    string currentTier = membership->getLevelName();

    // 1000 loyalty points -> Upgrade to next tier (Essential -> Extra -> Deluxe)
    if (loyaltypoints >= 1000)
    {
        if (currentTier == "Essential")
        {
            cout << "\n[MEMBERSHIP UPGRADE] Congratulations! You've earned Deluxe membership!" << endl;
            setMembership(new DeluxeMembership());
            loyaltypoints -= 1000;
            cout << "Deluxe membership activated! (Borrow up to 10 books, 50% fine discount, free waiver, priority queue)" << endl;
            return true;
        }
        else if (currentTier == "Extra")
        {
            cout << "\n[MEMBERSHIP UPGRADE] Congratulations! You've earned Deluxe membership!" << endl;
            setMembership(new DeluxeMembership());
            loyaltypoints -= 1000;
            cout << "Deluxe membership activated! (Borrow up to 10 books, 50% fine discount, free waiver, priority queue)" << endl;
            return true;
        }
        else if (currentTier == "Deluxe")
        {
            cout << "\n[INFO] You already have the highest membership tier (Deluxe)." << endl;
            return false;
        }
    }
    return false;
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

// update profile
void User::updateprofile(string firstName, string lastName, string email, string password)
{
    this->firstName = firstName;
    this->lastName = lastName;
    this->email = email;
    this->password = password;
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

void User::viewMyReservations() const
{
    // This is a placeholder that will be called from main_cli.cpp
    // Actual reservation viewing is handled through LibrarySystem
    cout << "\n[INFO] View your reservations through the main menu option.\n";
}
