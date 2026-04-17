#include "CategoryPreference.h"
#include <iostream>
using namespace std;

CategoryPreference::CategoryPreference() {
    category = "None";
}

CategoryPreference::CategoryPreference(string category) {
    if (isValidCategory(category)) {
        this->category = category;
    } else {
        this->category = "None";
    }
}

void CategoryPreference::setCategory(string category) {
    if (isValidCategory(category)) {
        this->category = category;
    } else {
        cout << "Invalid category. Setting to None." << endl;
        this->category = "None";
    }
}

string CategoryPreference::getCategory() const {
    return category;
}

// 🔥 simple validation logic
bool CategoryPreference::isValidCategory(string category) {
    if (category == "Fiction" ||
        category == "Science" ||
        category == "History" ||
        category == "Technology" ||
        category == "Other") {
        return true;
    }
    return false;
}

void CategoryPreference::showPreference() const {
    cout << "Preferred Category: " << category << endl;
}