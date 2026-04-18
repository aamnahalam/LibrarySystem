#ifndef REVIEW_H
#define REVIEW_H

#include <string>

class Review
{
public:
    Review();

    void addReview(const std::string& userId, const std::string& comment, int rating);
    std::string getComment() const;
    int getRating() const;

private:
    std::string comment;
    int rating;
};

#endif