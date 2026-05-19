#pragma once
#include <QString>
#include "data/ammo_types.h"
#include "data/armor_slots.h"
#include "data/char_types.h"
#include "data/damage_types.h"
#include "data/equipment_bonuses.h"
#include "data/skill_types.h"
#include "data/use_effects.h"

QString ammo_type_to_str(ammo_type type);
QString armor_slot_to_str(armor_slot slot);
QString char_type_to_str(char_type type);
QString damage_type_to_str(damage_type type);
QString equipment_bonus_to_str(equipment_bonus bonus);
QString skill_type_to_str(skill_type type);
QString use_effect_to_str(use_effect effect);
