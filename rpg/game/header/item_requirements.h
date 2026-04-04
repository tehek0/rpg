#pragma once
#include <vector>
#include "data/char_types.h"
#include "data/skill_types.h"

class base_requirement {
public:
    unsigned int required;
};

class char_requirement : public base_requirement {
public:
    char_requirement(unsigned int set_requirement, char_type char_): required(set_requirement), type(char_)
    {}
    char_type type;
};

class skill_requirement : public base_requirement {
public:
    skill_requirement(unsigned int set_requirement, skill_type skill): required(set_requirement), type(skill)
    {}
    skill_type type;
};

struct item_requirements {
    unsigned short min_level;
    std::vector<base_requirement*> item_requirements_ptrs;
    ~item_requirements();
};
