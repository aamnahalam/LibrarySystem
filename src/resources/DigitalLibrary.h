#ifndef DIGITALLIBRARY_H
#define DIGITALLIBRARY_H

#include <vector>
#include "Resource.h"

class DigitalLibrary
{
private:
    int accessAccount;
    int downloadLimit;
    vector<Resource*> resources;

public:
    DigitalLibrary();

    void viewOnline(int resourceID);
    void downloadPDF(int resourceID);
    void trackAccessCount();
};

#endif