#include <iostream>
#include "Membership.h"
using namespace std;

// Constructor definition for Membership class:
Membership::Membership(const string &levelName) : levelName(levelName) {}

// Method definition to get level Name:
string Membership::getLevelName() const
{
    return levelName;
}
