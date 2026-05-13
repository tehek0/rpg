#pragma once
#include "../rpg/game/inc/json.hpp"
#include "datatypes.hpp"
#include "QString"
using js = nlohmann::json;

namespace dev {
void set_ids_default_state();
unsigned long long throw_id(datatype type);
void remove_id(datatype type, unsigned long long id);
}
