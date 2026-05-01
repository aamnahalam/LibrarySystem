#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

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

public:
    Resource(int id, string title, string author, string category);

    virtual void displayDetails() = 0;
    void updateAvailability(bool status);

    string getTitle() const;

    virtual int getBorrowLimit() = 0;
    virtual double getFineRate() = 0;

    void addRating(int value);
    bool getAvailability();


    int getResourceID() const;
    bool getDigitalAvailable() const;

    bool operator>(const Resource& other) const;
};

#endif