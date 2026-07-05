#pragma once
#include "inc/json.hpp"
#include "data/datatypes.hpp"
#include "QString"
using js = nlohmann::ordered_json;

namespace dev {
void set_ids_default_state();
unsigned long long throw_id(datatype type);
void remove_id(datatype type, unsigned long long id);
}
