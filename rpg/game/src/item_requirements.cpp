#include "../header/item_requirements.h"

// Избегаем утечку памяти (небольшую)
item_requirements::~item_requirements() {
    for (size_t i = 0; i < item_requirements_ptrs.size(); ++i) {
        delete item_requirements_ptrs[i];
    }
}

bool item_requirements::operator==(item_requirements& other) {
    if (this->min_level != other.min_level || this->item_requirements_ptrs != other.item_requirements_ptrs)
        return false;

    return true;
}

bool item_requirements::operator!=(item_requirements& other) {
    if (this->min_level != other.min_level || this->item_requirements_ptrs != other.item_requirements_ptrs)
        return true;

    return false;
}

