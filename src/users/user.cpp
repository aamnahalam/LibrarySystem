#include "user.h"
#include "Resource.h"
#include "BorrowRecord.h"
#include "Membership.h"
#include <iostream>
using namespace std;
int convertDate(string date){
    if(date.size() != 10){
        cout<<" Invalid Date Format. "<<endl;
        return 0;
    } 
  int year=stoi(date.substr(0,4));
  int month=stoi(date.substr(5,2));
  int day=stoi(date.substr(8,2));
    return year*10000+month*100+day;

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

string formatDate(int date)
{
    int year = date / 10000;
    int month = (date / 100) % 100;
    int day = date % 100;

    string m = (month < 10 ? "0" : "") + to_string(month);
    string d = (day < 10 ? "0" : "") + to_string(day);

    return to_string(year) + "-" + m + "-" + d;
}

int getTotalDays(int dateInt) {
    int year = dateInt / 10000;
    int month = (dateInt / 100) % 100;
    int day = dateInt % 100;

    //30-day assumption consistently
    return (year * 360) + (month * 30) + day;
}
//  Add Balance
User::User(int id, string firstName, string lastName, string email, string password
, double balance)
    : Person(id, firstName, lastName,email,password) {
        accountbalance=balance;
        loyaltypoints=0;
        membership=nullptr;
        readingFrequency=0;
        
    }
 // Set membership
void User::setMembership(Membership*m){
    membership=m;
}
// Borrow Resource
bool User::borrowresources(Resource* r, string date){
    if (accountbalance < 0) {
        cout << "Access Denied: You have unpaid fines (" << accountbalance << "). Please recharge." << endl;
        return false;
    }
    if(r==nullptr){
    cout<<" Invalid Resource. "<< endl;
    return false;
    }
    if(!r->getAvailability()){
        cout<<" Resource Not Available. "<<endl;
        return false;
    }

    // Check Membership Limit
    if(membership!=nullptr){
        int limit=membership->getMaxBorrowLimit();
        if((int)borrowedResources.size()>=limit){
            cout<< "Borrow Limit Exceeded. "<<endl;
            return false;
        }
    }
    borrowedResources.push_back(r);
    r->updateAvailability(false);

    cout<< " Borrowing Resource: "<<r->getTitle()<<endl;
    cout<< " Borrow Date: "<<date<<endl;

   int borrowDate = convertDate(date);
   if(borrowDate == 0){
    cout << "Invalid Borrow Date.\n";
    return false;
}
  int dueInt = addDays(borrowDate, 7);
  string duedatestr = formatDate(dueInt);
    cout<< " Due Date: "<<duedatestr<<endl;


    // Borrow record
    BorrowRecord record(this, r->getTitle(), date, duedatestr);
    borrowHistory.push_back(record);
    readingFrequency++;
    cout<<" Resource Borrowed Successfully. "<<endl;
    return true;
}

// Return Resource 
double User::returnresources(Resource* r, string date){
    bool found=false;
    for(int i=0; i<(int)borrowedResources.size(); i++){
        if (borrowedResources[i]==r){
            borrowedResources.erase(borrowedResources.begin()+i);
            r->updateAvailability(true);
            found=true;
            break;
        }
    }
    if(!found){
        cout<< " Resource Not Found In Borrowed List. "<<endl;
        return 0.0;
    }
    
    bool historyFound=false;
    // Update Borrow Record
    for(int i = 0; i < (int)borrowHistory.size(); i++){
        
        if(borrowHistory[i].getResourceName() == r->getTitle() &&
        !borrowHistory[i].getReturnStatus()){
            
            historyFound = true;
            cout<< " Resource Returned Successfully. "<<endl;

            int returnDate = convertDate(date);
            if(returnDate == 0){
    cout << "Invalid Return Date.\n";
    return 0.0;
              }
            int dueDate = convertDate(borrowHistory[i].getDueDate());

            borrowHistory[i].markAsReturned(date);

            if(returnDate > dueDate){
                int latedays = getTotalDays(returnDate) - getTotalDays(dueDate);
                if(latedays < 0) latedays = 0;
                
                double finePerDay = 10.0;
                double fine = latedays * finePerDay;

                if(membership != nullptr){
                    fine *= (1 - membership->getFineDiscount());
                }

                cout << "Late Return. Days Late: " << latedays << endl;
                cout << "Fine Charged: " << fine << endl;

                // Debt Logic
                accountbalance -= fine;
                cout << "Fine Paid. Remaining Balance: " << accountbalance << endl;
                
                if(accountbalance < 0){
                    cout << "Warning: Your Account Is Now In Debt. Please Recharge." << endl;
                }
                return fine;
            } 
            else {
                cout << " Resource Returned On Time. No Fine. " << endl;
            }

            return 0.0; // exit loop after finding the record
        }
    }

    if(!historyFound){
        cout << "Resource Not Found In Borrow History.\n";
    }

    return 0.0;
}

//  Add Balance
void User::rechargebalance(double amount) {
    if(amount<=0){
        cout<<" Invalid Amount. "<<endl;
        return;
    }
    accountbalance+=amount;
    cout<<" Balance Recharged Successfully. "<<endl;
}

// Deduct from balance
bool User::deductFromBalance(double amount)
{
    if (accountbalance >= amount)
    {
        accountbalance -= amount;
        return true;
    }
    return false;
}

// Add points
void User::earnpoints(int points)
{
    loyaltypoints += points;
}

int User::getID()const{
    return id;
}
string User::getEmail() const
{
    return email;
}
string User::getFullName() const
{
    return firstName + " " + lastName;
}

double User::getAccountBalance() const
{
    return accountbalance;
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





