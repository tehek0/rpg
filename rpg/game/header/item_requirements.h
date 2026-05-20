#pragma once
#include <QString>
#include <vector>
#include "data/char_types.h"
#include "data/skill_types.h"

class base_requirement {
public:
    unsigned int required;
    base_requirement(unsigned int set_requirement): required(set_requirement)
    {}
    virtual QString text_requirement() = 0;
    virtual ~base_requirement() = default;
    virtual bool operator==(base_requirement* other);
    virtual bool operator!=(base_requirement* other);
};

class char_requirement : public base_requirement {
public:
    char_requirement(unsigned int set_requirement, char_type char_): base_requirement(set_requirement), type(char_)
    {}
    char_type type;
    virtual QString text_requirement();
    virtual ~char_requirement() = default;
    bool operator==(char_requirement* other);
    bool operator!=(char_requirement* other);
    bool operator==(base_requirement* other);
    bool operator!=(base_requirement* other);
};

class skill_requirement : public base_requirement {
public:
    skill_requirement(unsigned int set_requirement, skill_type skill): base_requirement(set_requirement), type(skill)
    {}
    skill_type type;
    virtual QString text_requirement();
    virtual ~skill_requirement() = default;
    bool operator==(skill_requirement* other);
    bool operator!=(skill_requirement* other);
    bool operator==(base_requirement* other);
    bool operator!=(base_requirement* other);
};

struct item_requirements {
    unsigned short min_level = 0;
    std::vector<base_requirement*> item_requirements_ptrs;
    ~item_requirements();
    bool operator==(item_requirements& other);
    bool operator!=(item_requirements& other);
};
