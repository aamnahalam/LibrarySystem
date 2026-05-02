#include "user.h"
#include "Resource.h"
#include "BorrowRecord.h"
#include "Membership.h"
#include <iostream>
#include <stdexcept>
using namespace std;

User::User(int id, string firstName, string lastName, string email, string password, double balance)
    : Person(id, firstName, lastName,email,password) {
        accountbalance=balance;
        loyaltypoints=0;
        membership=nullptr;
        readingFrequency=0;
        isLocked=false;
    }

User::~User() {
    delete membership;    // Clean up any dynamically allocated resources if needed
}

void User::lock() {
    isLocked = true;
    cout << "User account locked due to unpaid fines." << endl;
}

bool User::getLockStatus() const {
    return isLocked;
}

bool User::borrowresources(Resource* r, string date) {
    if (isLocked) {
        cout << "Access Denied: Your account is locked due to unpaid fines. Please recharge." << endl;
        return false;
    }
    if (!r || !r->getAvailability()) {
        cout << "Access Denied: You have unavailable resource." << endl;
        return false;
    }
    if (membership && (int)borrowedResources.size() >= membership->getMaxBorrowLimit()) {
        cout << "Access Denied: You have reached your borrow limit." << endl;
        return false;
    }
    borrowedResources.push_back(r);
    r->updateAvailability(false);
    string duedate = formatDate(addDays(convertDate(date), 7)); // Assuming 7-day borrow period
    cout << "Borrowing Resource: " << r->getTitle() << endl;
    cout << "Borrow Date: " << date << endl;
    cout << "Due Date: " << duedate << endl;
    borrowHistory.emplace_back(this, r->getTitle(), date, duedate); // Due date will be set in BorrowRecord constructor
    return true;
}
bool User::deductFromBalance(double amount) {
    if (accountbalance >= amount) {
        accountbalance -= amount;
        return true;
    }
    return false;
}
double User::returnresources(Resource* r, string date) {
    for (auto it = borrowedResources.begin(); it != borrowedResources.end(); ++it) {
        if (*it == r) {
            borrowedResources.erase(it);
            r->updateAvailability(true);
            // Update the corresponding borrow record
            for (auto& record : borrowHistory) {
                if (record.getResourceName() == r->getTitle() && !record.getReturnStatus()) {
                    record.markAsReturned(date);
                    break;
                }
            }
            return 0.0; // Assuming no fine logic implemented here for simplicity
        }
    }
    cout << "Resource not found in borrowed list." << endl;
    return 0.0;
}
void User::rechargebalance(double amount) {
    accountbalance += amount;
    cout << "Balance recharged successfully. Current balance: " << accountbalance << endl;
}
void User::earnpoints(int points) {
    loyaltypoints += points;
    cout << "Loyalty points earned: " << points << endl;
}
void User::setMembership(Membership* m) {
    if (membership) {
        delete membership;
    }
    membership = m;
}

double User::getAccountBalance() const {
    return accountbalance;
}
int User::getID()const{
    return id;
}
string User::getEmail() const {
    return email;
}
string User::getFullName() const {
    return firstName + " " + lastName;
}

string User::getPreferredCategory() const {
    if (favouriteCategories.empty()) {
        return "";
    }
    return favouriteCategories[0];
}

const vector<BorrowRecord>& User::getBorrowHistory() const { return borrowHistory; }

int convertDate(string date) {
    if (date.size() != 10) {
        cout << " Invalid Date Format. Use YYYY-MM-DD" << endl;
        return 0;
    }

    try {
        // stoi will throw invalid_argument if it hits a letter
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        // You can manually throw an error if the numbers are nonsense
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            throw out_of_range("Month or Day is logically incorrect");
        }

        return year * 10000 + month * 100 + day;
    } 
    catch (const invalid_argument& e) {
        cerr << "Data Error: Date contains non-numeric characters: " << e.what() << endl;
        return 0;
    } 
    catch (const out_of_range& e) {
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
    while (day > 30) {
        day -= 30;
        month++;
    }

    while (month > 12) {
        month -= 12;
        year++;
    }

    return year * 10000 + month * 100 + day;
}

int getTotalDays(int dateInt) {
    int year = dateInt / 10000;
    int month = (dateInt / 100) % 100;
    int day = dateInt % 100;

    //30-day assumption consistently
    return (year * 360) + (month * 30) + day;
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
void User::viewhistory() {
    if(borrowHistory.empty()){
        cout<< " No Borrow History. "<<endl;
        return;
    }
    cout<< " Borrow History: "<<endl;
    for(int i=0; i<(int)borrowHistory.size(); i++){
        borrowHistory[i].showRecord();
    }
}

// update profile
void User::updateprofile(string firstName, string lastName, string email, string password)  {
    this->firstName=firstName;
    this->lastName=lastName;
    this->email=email;
    this->password=password;
    cout<< " Profile Updated Successfully. "<<endl;
}

void User::displayInfo() {
    cout<<  "User Information: "<<endl;
    cout<<  "-----------------------------"<<endl;
    cout<<  "ID: "<<id<<endl;
    cout << "User Name: " << firstName << " " << lastName << endl;
    cout << "Email: " <<email<<endl;
    cout << "Balance: "<<accountbalance<<endl;
    cout << "Loyalty Points: " << loyaltypoints<<endl;
    cout << "Membership Level: " << (membership ? membership->getLevelName() : "None") << endl;
    cout<<  "Borrowed Resources: "<<borrowedResources.size()<<endl;
    cout<<  "-----------------------------"<<endl;

    if(membership!=nullptr){
        membership->displayDetails();
    }
}

//operator overload for user comparison
bool User::operator==(const User& other) const {
    return id==other.id;
}
