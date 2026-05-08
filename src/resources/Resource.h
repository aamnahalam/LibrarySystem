#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>
using namespace std;

class Admin; // forward declaration
class LibrarySystem; // forward declaration
class Review; // forward declaration

class Resource
{
protected:
    int resourceID;
    string title;
    string author;
    string category;
    bool isAvailable;
    double rating;
    double price;
    int borrowDuration;
    int borrowCount;
    bool isNewArrival;
    vector<Review*> reviews;
    double averageRating;
    bool digitalAvailable;

public:
    Resource(int id, string title, string author, string category);

    virtual void displayDetails() = 0;
    virtual ~Resource();
    void updateAvailability(bool status);
    string getTitle() const;
    string getAuthor() const;

    virtual double getFineRate() = 0;
    
    void addRating(int value);
    void incrementBorrowCount();
    bool getAvailability();
    string getCategory() const;
    double getReviewScore() const;
    double getRating() const;
    int getBorrowCount() const;
    bool getIsNewArrival() const;

    int getResourceID() const;

    bool operator>(const Resource& other) const;

    // Review Methods
    void addReview(Review* review);
    double getAverageRating() const;
    void displayReviews() const;
    vector<Review*> getReviews() const;
    int getReviewCount() const;

    // Friend class for Admin and LibrarySystem access
    friend class Admin;
    friend class LibrarySystem;
};

#endif