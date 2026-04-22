#include "Review.h"
#include "user.h"
#include <string>
// Definition of Constructor:
Review::Review(int ratingValue, const string &reviewText, User *user) : ratingValue(ratingValue), reviewText(reviewText), user(user), isHidden(false)
{
}
// Definition of Destructor:
Review::~Review()
{
    user = nullptr;
}
void Review::addReview(const string &text, int rating)
{
    reviewText = text;
    ratingValue = rating;
    isHidden = false;
}
void Review::hideReview()
{
    isHidden = true;
}
// Definition of setters:
void Review::setRatingValue(int rating)
{
    ratingValue = rating;
}
void Review::setReviewText(const string &text)
{
    reviewText = text;
}
// Definition of getters:
int Review::getRatingValue() const
{
    return ratingValue;
}
string Review::getReviewText() const
{
    return reviewText;
}
User *Review::getUser() const
{
    return user;
}
bool Review::isHiddenReview() const
{
    return isHidden;
}
// Definition of utility functions:
string Review::toString() const
{
    string status = isHidden ? "Hidden" : "Visible";
    return "Review(Rating: " + to_string(ratingValue) + ", Status: " + status + ")\nText: " + reviewText;
}
void Review::getUserName() const
{
    if (user != nullptr)
    {
        // Calling method of User class
        user->displayInfo();
    }
}