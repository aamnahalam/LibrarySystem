#pragma once
#ifndef FINEWALLETMANAGER_H
#define FINEWALLETMANAGER_H

#include <string>
#include "user.h"

class FineWalletManager
{
public:
    // Constructors:
    FineWalletManager();
    FineWalletManager(double initialFine, int days);

    // Fine related
    double calculateFine(int daysLate, double rate);
    bool deductFromBalance(User *user, double amount);
    void rechargeWallet(User *user, double amount);

    // Setters:
    void setFineAmount(double amount);
    void setOverdueDays(int days);

    // Getters:
    double getFineAmount() const;
    int getOverdueDays() const;

private:
    double fineAmount;
    int overdueDays;
};

#endif