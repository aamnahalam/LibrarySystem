#include "FineWalletManager.h"
#include "../users/user.h"
#include "../exceptions/LibraryException.h"
#include "../exceptions/InsufficientBalanceException.h"
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
    if (!user)
        throw InsufficientBalanceException("Invalid user pointer");
    if (user->getAccountBalance() >= amount && user->deductFromBalance(amount))
    {
        cout << "Deducted $" << amount << " from user " << user->getID() << endl;
        return true;
    }
    throw InsufficientBalanceException("User " + to_string(user->getID()) + " has insufficient balance.");
}
// Recharging wallet:
void FineWalletManager::rechargeWallet(User *user, double amount)
{
    if (!user)
        throw LibraryException("Invalid user pointer");
    if (amount <= 0)
        throw LibraryException("Recharge amount must be positive");
    user->rechargebalance(amount);
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