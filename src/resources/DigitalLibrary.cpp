#include "DigitalLibrary.h"
#include <iostream>
using namespace std;

void DigitalLibrary::addResource(Resource* r) {
    resources.push_back(r);
}

void DigitalLibrary::showAll() {
    for (auto r : resources) {
        r->display();
    }
}

Resource* DigitalLibrary::findById(int id) {
    for (auto r : resources) {
        // simple check if needed later
    }
    return nullptr;
}