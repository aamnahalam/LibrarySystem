#ifndef NORMALMEMBERSHIP_H
#define NORMALMEMBERSHIP_H

#include "Membership.h"

class NormalMembership : public Membership
{
public:
    NormalMembership();

    void renewMembership() override;
};

#endif