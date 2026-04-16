#include "DigitalLibrary.h"
#include <iostream>

DigitalLibrary::DigitalLibrary()
{
    accessAccount = 0;
    downloadLimit = 5;
}

void DigitalLibrary::viewOnline(int resourceID)
{
    cout << "Viewing resource online with ID: " << resourceID << endl;
    accessAccount++;
}

void DigitalLibrary::downloadPDF(int resourceID)
{
    if(downloadLimit > 0)
    {
        cout << "Downloading PDF of resource ID: " << resourceID << endl;
        downloadLimit--;
    }
    else
    {
        cout << "Download limit reached.\n";
    }
}

void DigitalLibrary::trackAccessCount()
{
    cout << "Total accesses: " << accessAccount << endl;
}
