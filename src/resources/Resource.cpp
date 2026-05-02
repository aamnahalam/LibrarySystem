#include "Resource.h"
#include <iostream>

Resource::Resource(int id, string title, string author, string category)
{
    resourceID = id;
    this->title = title;
    this->author = author;
    this->category = category;

    isAvailable = true;
    rating = 0;
    digitalAvailable = false;
    price = 0;
    borrowDuration = 0;
    borrowCount = 0;
    isNewArrival = true;
}

string Resource::getTitle() const {
    return title;
}

void Resource::updateAvailability(bool status)
{
    isAvailable = status;
}

void Resource::addRating(int value)
{
    rating = (rating + value) / 2;
}

bool Resource::getAvailability(){
    return isAvailable;
}

string Resource::getCategory() const {
    return category;
}

double Resource::getReviewScore() const {
    return rating;
}

int Resource::getBorrowCount() const {
    return borrowCount;
}

bool Resource::getIsNewArrival() const {
    return isNewArrival;
}

bool Resource::operator>(const Resource& other) const
{
    return rating > other.rating;
}

int Resource::getResourceID() const { 
    return resourceID; 
} 

bool Resource::getDigitalAvailable() const { 
    return digitalAvailable; 
} 