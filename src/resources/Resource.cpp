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
}

void Resource::updateAvailability(bool status)
{
    isAvailable = status;
}

void Resource::addRating(int value)
{
    rating = (rating + value) / 2;
}

bool Resource::operator>(Resource& other)
{
    return rating > other.rating;
}
