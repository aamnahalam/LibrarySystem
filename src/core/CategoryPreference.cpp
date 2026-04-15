#include "CategoryPreference.h"
#include <iostream>
using namespace std;

CategoryPreference::CategoryPreference() {
    category = "None";
}

CategoryPreference::CategoryPreference(string category) {
    this->category = category;
}

void CategoryPreference::setCategory(string category) {
    this->category = category;
}

string CategoryPreference::getCategory() {
    return category;
}

void CategoryPreference::showPreference() {
    cout << "Preferred Category: " << category << endl;
}