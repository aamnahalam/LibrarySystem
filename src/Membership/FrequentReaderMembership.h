#ifndef FREQUENTREADERMEMBERSHIP_H
#define FREQUENTREADERMEMBERSHIP_H

#include "Membership.h"

class FrequentReaderMembership : public Membership
{
public:
    FrequentReaderMembership();

    void renewMembership() override;
};

#endif
