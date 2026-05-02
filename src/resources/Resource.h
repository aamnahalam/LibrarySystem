#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

class Admin; // forward declaration

class Resource
{
protected:
    int resourceID;
    string title;
    string author;
    string category;
    bool isAvailable;
    double rating;
    bool digitalAvailable;
    double price;
    int borrowDuration;
    int borrowCount;
    bool isNewArrival;

public:
    Resource(int id, string title, string author, string category);

    virtual void displayDetails() = 0;
    virtual ~Resource();
    void updateAvailability(bool status);

    string getTitle() const;

    virtual int getBorrowLimit() = 0;
    virtual double getFineRate() = 0;

    void addRating(int value);
    bool getAvailability();
    string getCategory() const;
    double getReviewScore() const;
    int getBorrowCount() const;
    bool getIsNewArrival() const;

    int getResourceID() const;
    bool getDigitalAvailable() const;

    bool operator>(const Resource& other) const;

    // Friend class for Admin and LibrarySystem access
    friend class Admin;
    friend class LibrarySystem;
};

#endif