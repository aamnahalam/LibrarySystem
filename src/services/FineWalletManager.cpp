#include "FineWalletManager.h"
#include "LibraryException.h"
#include "InsufficientBalanceException.h"
#include <iostream>
#include <string>
// Default Constructor:
FineWalletManager::FineWalletManager() : fineAmount(0.0), overdueDays(0)
{
}
// Parametrized Constructor:
FineWalletManager::FineWalletManager(double initialFine, int days) : fineAmount(initialFine), overdueDays(days)
{
}
// Calculating fine:
double FineWalletManager::calculateFine(int daysLate, double rate)
{
    return daysLate * rate;
}
// Deducting fine from balance:
bool FineWalletManager::deductFromBalance(User *user, double amount)
{
    if (user == nullptr)
    {
        throw InsufficientBalanceException("Invalid user pointer");
    }
    if (user->getAccountBalance() >= amount)
    {
        bool success = user->deductFromBalance(amount);
        if (success)
        {
            cout << "Successfully deducted $" << amount << " from user: " << user->getID() << endl;
            return true;
        }
    }
    // to_string() is used to convert different datatypes to string
    throw InsufficientBalanceException("User " + to_string(user->getID()) + " has insufficient balance."
                                                                            "Current balance: $" +
                                       to_string(user->getAccountBalance()) + ", Required: $" + to_string(amount));
}
// Recharging wallet:
void FineWalletManager::rechargeWallet(User *user, double amount)
{
    if (user == nullptr)
    {
        throw LibraryException("Invalid user pointer for recharge");
    }
    if (amount <= 0)
    {
        throw LibraryException("Recharge amount must be positive");
    }
    // Calling recharge balance of user class
    user->rechargebalance(amount);
    cout << "Successfully recharged $" << amount << " to user" << user->getID() << endl;
    cout << "New balance: $" << user->getAccountBalance() << endl;
}
// Setting fine amount:
void FineWalletManager::setFineAmount(double amount)
{
    fineAmount = amount;
}
// Setting overdue days:
void FineWalletManager::setOverdueDays(int days)
{
    overdueDays = days;
}
// Getting fine amount:
double FineWalletManager::getFineAmount() const
{
    return fineAmount;
}
// Getting overdue days:
int FineWalletManager::getOverdueDays() const
{
    return overdueDays;
}