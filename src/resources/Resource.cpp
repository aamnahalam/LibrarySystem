#include "Resource.h"
#include <iostream>
using namespace std;

Resource::Resource(int id, string title) {
    this->id = id;
    this->title = title;
    this->available = true;
}

void Resource::borrow() {
    available = false;
}

void Resource::giveBack() {
    available = true;
}

bool Resource::isAvailable() {
    return available;
}

void Resource::display() {
    cout << "Resource: " << title << endl;
}