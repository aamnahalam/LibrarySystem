#ifndef FINEWALLETMANAGER_H
#define FINEWALLETMANAGER_H

#include <string>

class FineWalletManager
{
public:
    FineWalletManager();

    // Fine related
    void calculateFine(int daysLate);
    double getFineAmount() const;

    // Wallet related
    void addBalance(double amount);
    void deductBalance(double amount);
    double getBalance() const;

    // Payment
    void payFine();

private:
    double fineAmount;
    double balance;
};

#endif