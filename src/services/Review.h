#ifndef REVIEW_H
#define REVIEW_H

#include <string>
#include <vector>
using namespace std;

// Forward declaration of user class:
class User;

class Review
{
public:
    // Constructor:
    Review(int ratingValue, const string &reviewText, User *user);

    // Destructor:
    ~Review();

    // Method to add review:
    void addReview(const string &text, int rating);

    // Method to hide review:
    void hideReview();

    // Setters:
    void setRatingValue(int rating);
    void setReviewText(const string &text);

    // Getters
    int getRatingValue() const;
    string getReviewText() const;
    User *getUser() const;
    bool isHiddenReview() const;

    // Utility methods
    string toString() const;
    void getUserName() const;

private:
    int ratingValue;
    string reviewText;
    User *user;
    bool isHidden;
};

#endif