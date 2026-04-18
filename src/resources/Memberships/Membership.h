#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <string>

class Membership
{
public:
    Membership();

    virtual void renewMembership();
    virtual void cancelMembership();

    void setStartDate(const std::string& date);
    std::string getStartDate() const;

    void setEndDate(const std::string& date);
    std::string getEndDate() const;

protected:
    std::string startDate;
    std::string endDate;
    bool active;
};

#endif