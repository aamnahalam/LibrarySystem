#include "Review.h"
#include "user.h"
#include <string>

Review::Review(int ratingValue, const string &reviewText, User *user) : ratingValue(ratingValue), reviewText(reviewText), user(user), isHidden(false)
{
}
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
void Review::setRatingValue(int rating)
{
    ratingValue = rating;
}
void Review::setReviewText(const string &text)
{
    reviewText = text;
}
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
string Review::toString() const
{
    string status = isHidden ? "Hidden" : "Visible";
    return "Review(Rating: " + to_string(ratingValue) + ", Status: " + status + ")\nText: " + reviewText;
}
void Review::getUserName() const
{
    if (user != nullptr)
    {
        user->displayInfo();
    }
}