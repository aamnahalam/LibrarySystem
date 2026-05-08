#include "Resource.h"
#include "../services/Review.h"
#include "../users/user.h"
#include <iostream>

Resource::~Resource()
{
}

Resource::Resource(int id, string title, string author, string category)
{
    resourceID = id;
    this->title = title;
    this->author = author;
    this->category = category;

    isAvailable = true;
    rating = 0;
    price = 0;
    borrowDuration = 0;
    borrowCount = 0;
    isNewArrival = true;
    averageRating = 0.0;
    digitalAvailable = false;
}
void Resource::incrementBorrowCount()
{
    borrowCount++;
    isNewArrival = false;
}

string Resource::getTitle() const
{
    return title;
}

string Resource::getAuthor() const {
    return author;
}

void Resource::updateAvailability(bool status)
{
    isAvailable = status;
}

void Resource::addRating(int value)
{
    rating = (rating + value) / 2;
}

bool Resource::getAvailability()
{
    return isAvailable;
}

string Resource::getCategory() const
{
    return category;
}

double Resource::getReviewScore() const
{
    return rating;
}
double Resource::getRating() const
{
    return rating;
}
int Resource::getBorrowCount() const
{
    return borrowCount;
}

bool Resource::getIsNewArrival() const
{
    return isNewArrival;
}

bool Resource::operator>(const Resource &other) const
{
    return rating > other.rating;
}

int Resource::getResourceID() const
{
    return resourceID;
}

void Resource::addReview(Review* review)
{
    if (review != nullptr) {
        reviews.push_back(review);
        
        // Recalculate average rating
        double sum = 0;
        for (Review* r : reviews) {
            if (!r->isHiddenReview()) {
                sum += r->getRatingValue();
            }
        }
        int visibleCount = 0;
        for (Review* r : reviews) {
            if (!r->isHiddenReview()) visibleCount++;
        }
        averageRating = visibleCount > 0 ? sum / visibleCount : 0.0;
        rating = averageRating;
    }
}

double Resource::getAverageRating() const
{
    return averageRating;
}

void Resource::displayReviews() const
{
    if (reviews.empty()) {
        cout << "\nNo reviews yet for this resource.\n";
        return;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << "REVIEWS FOR: " << title << "\n";
    cout << "Average Rating: ";
    for (int i = 0; i < (int)averageRating; i++) cout << "*";
    cout << " (" << averageRating << "/5) - " << getReviewCount() << " reviews\n";
    cout << string(60, '=') << "\n";

    int count = 0;
    for (const Review* review : reviews) {
        if (!review->isHiddenReview()) {
            count++;
            cout << "\n" << count << ". ";
            for (int i = 0; i < review->getRatingValue(); i++) cout << "*";
            cout << " (" << review->getRatingValue() << "/5)\n";
            cout << "   By: " << review->getUser()->getFullName() << "\n";
            cout << "   \"" << review->getReviewText() << "\"\n";
        }
    }
}

vector<Review*> Resource::getReviews() const
{
    return reviews;
}

int Resource::getReviewCount() const
{
    int count = 0;
    for (const Review* r : reviews) {
        if (!r->isHiddenReview()) count++;
    }
    return count;
}