#include "../header/item_requirements.h"
#include "../header/enum_translation.h"
#include "../header/character.h"

item_requirements::~item_requirements() {
    for (size_t i = 0; i < item_requirements_ptrs.size(); ++i) {
        delete item_requirements_ptrs[i];
        item_requirements_ptrs[i] = nullptr;
    }
}

item_requirements::item_requirements(item_requirements& other) {
    item_requirements_ptrs.reserve(other.item_requirements_ptrs.size());
    for (const auto elem : other.item_requirements_ptrs) {
        item_requirements_ptrs.emplace_back(elem->clone());
    }
}

bool item_requirements::match_all(entity_stats stats) {
    for (auto elem : this->item_requirements_ptrs) {
        if (!elem->match_requirement(stats)) {
            return false;
        }
    }
    return true;
}

base_requirement* skill_requirement::clone() {
    return new skill_requirement(this->required, this->type);
}

base_requirement* char_requirement::clone() {
    return new char_requirement(this->required, this->type);
}

bool base_requirement::is_char_requirement() {
    return false;
}
bool base_requirement::is_skill_requirement() {
    return false;
}


bool base_requirement::operator==(base_requirement* other) {
    return false;
}
bool base_requirement::operator!=(base_requirement* other) {
    return false;
}

bool skill_requirement::is_skill_requirement() {
    return true;
}

bool skill_requirement::match_requirement(entity_stats stats) {
    return this->required <= stats.get_stat(type);
}

QString skill_requirement::text_requirement() {
    QString text_requirement = QString("%1 %2").arg(skill_type_to_str(type)).arg(required);
    return text_requirement;
}

bool skill_requirement::operator==(skill_requirement* other) {
    if (this->required != other->required || this->type != other->type)
        return false;

    return true;
}
bool skill_requirement::operator!=(skill_requirement* other) {
    if (this->required != other->required || this->type != other->type)
        return true;

    return false;
}
bool skill_requirement::operator==(base_requirement* other) {
    if (other->is_skill_requirement()) {
        return *this == static_cast<skill_requirement*>(other);
    }
    return false;
}
bool skill_requirement::operator!=(base_requirement* other){
    if (other->is_skill_requirement()) {
        return *this == static_cast<skill_requirement*>(other);
    }
    return true;
}

QString char_requirement::text_requirement() {
    QString text_requirement = QString("%1 %2").arg(char_type_to_str(type)).arg(required);
    return text_requirement;
}

bool char_requirement::is_char_requirement() {
    return true;
}

bool char_requirement::match_requirement(entity_stats stats) {
    return this->required <= stats.get_stat(type);
}

bool char_requirement::operator==(char_requirement* other) {
    if (this->required != other->required || this->type != other->type)
        return false;

    return true;
}
bool char_requirement::operator!=(char_requirement* other) {
    if (this->required != other->required || this->type != other->type)
        return true;

    return false;
}
bool char_requirement::operator==(base_requirement* other) {
    if (other->is_char_requirement()) {
        return *this == static_cast<char_requirement*>(other);
    }
    return false;
}
bool char_requirement::operator!=(base_requirement* other){
    if (other->is_char_requirement()) {
        return *this != static_cast<char_requirement*>(other);
    }
    return true;
}

bool item_requirements::operator==(item_requirements& other) {
    if (this->min_level != other.min_level || this->item_requirements_ptrs.size() != other.item_requirements_ptrs.size())
        return false;
    for (size_t i = 0; i < this->item_requirements_ptrs.size(); ++i) {
        if (*(this->item_requirements_ptrs[i]) != other.item_requirements_ptrs[i])
            return false;
    }

    return true;
}

bool item_requirements::operator!=(item_requirements& other) {
    if (this->min_level != other.min_level || this->item_requirements_ptrs.size() != other.item_requirements_ptrs.size())
        return true;
    for (size_t i = 0; i < this->item_requirements_ptrs.size(); ++i) {
        if (*(this->item_requirements_ptrs[i]) != other.item_requirements_ptrs[i])
            return true;
    }

    return false;
}
