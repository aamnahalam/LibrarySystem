#ifndef DIGITAL_LIBRARY_H
#define DIGITAL_LIBRARY_H

#include <vector>
#include "Resource.h"
using namespace std;

class DigitalLibrary {
private:
    vector<Resource*> resources;

public:
    void addResource(Resource* r);
    void showAll();
    Resource* findById(int id);
};

#endif