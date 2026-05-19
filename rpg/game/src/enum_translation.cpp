#include "../header/enum_translation.h"

QString ammo_type_to_str(ammo_type type) {
    switch(type) {
    case ammo_type::none: {
        return "-";
    }
    case ammo_type::pistol: {
        return "Пистолетный";
    }
    case ammo_type::shotgun: {
        return "Дробь";
    }
    case ammo_type::rifle: {
        return "Винтовочный";
    }
    case ammo_type::sniper: {
        return "Снайперский";
    }
    case ammo_type::minigun: {
        return "Пулемётный";
    }
    case ammo_type::energy: {
        return "Плазменный";
    }
    case ammo_type::energy_shotgun: {
        return "Плазменная дробь";
    }
    case ammo_type::rocket: {
        return "Ракета";
    }
    default: return "NULL";
    }
}

QString armor_slot_to_str(armor_slot slot) {
    switch(slot) {
    case armor_slot::head: {
        return "Голова";
    }
    case armor_slot::body: {
        return "Тело";
    }
    case armor_slot::legs: {
        return "Ноги";
    }
    default: return "NULL";
    }
}

QString char_type_to_str(char_type type) {
    switch(type) {
    case char_type::agility: {
        return "Ловкость";
    }
    case char_type::endurance: {
        return "Стойкость";
    }
    case char_type::intelligence: {
        return "Интеллект";
    }
    case char_type::luck: {
        return "Удача";
    }
    case char_type::strength: {
        return "Сила";
    }
    default: return "NULL";
    }
}

QString damage_type_to_str(damage_type type) {
    switch(type) {
    case damage_type::bullet: {
        return "Огнестрел.";
    }
    case damage_type::energy: {
        return "Энерг.";
    }
    case damage_type::melee: {
        return "Ближ.";
    }
    case damage_type::unarmed: {
        return "Кулач.";
    }
    case damage_type::explosive: {
        return "Взрывн.";
    }
    case damage_type::non_specified: {
        return "-";
    }
    default: return "NULL";
    }
}

QString equipment_bonus_to_str(equipment_bonus bonus) {
    switch(bonus) {
    case equipment_bonus::none: {
        return "-";
    }
    case equipment_bonus::change_armor: {
        return "Броня: %1";
    }
    case equipment_bonus::change_max_health: {
        return "Запас здоровья: %1";
    }
    case equipment_bonus::change_max_energy: {
        return "Запас энергии: %1";
    }
    case equipment_bonus::change_char_strength: {
        return "Сила: %1";
    }
    case equipment_bonus::change_char_agility: {
        return "Ловкость: %1";
    }
    case equipment_bonus::change_char_endurance: {
        return "Стойкость: %1";
    }
    case equipment_bonus::change_char_intelligence: {
        return "Интеллект: %1";
    }
    case equipment_bonus::change_char_luck: {
        return "Удача: %1";
    }
    case equipment_bonus::change_skill_guns: {
        return "Оружие: %1";
    }
    case equipment_bonus::change_skill_big_guns: {
        return "Тяжёлое оружие: %1";
    }
    case equipment_bonus::change_skill_unarmed: {
        return "Кулачный бой: %1";
    }
    case equipment_bonus::change_skill_science: {
        return "Наука: %1";
    }
    case equipment_bonus::change_skill_speech: {
        return "Красноречие: %1";
    }
    case equipment_bonus::change_skill_barter: {
        return "Бартер: %1";
    }
    case equipment_bonus::change_skill_survival: {
        return "Выживание: %1";
    }
    }
}

QString skill_type_to_str(skill_type type) {
    switch(type) {
    case skill_type::barter: {
        return "Бартер";
    }
    case skill_type::big_guns: {
        return "Тяжёлое оружие";
    }
    case skill_type::guns: {
        return "Оружие";
    }
    case skill_type::science: {
        return "Наука";
    }
    case skill_type::speech: {
        return "Красноречие";
    }
    case skill_type::survival: {
        return "Выживание";
    }
    case skill_type::unarmed: {
        return "Кулачный бой";
    }
    default: return "NULL";
    }
}

QString use_effect_to_str(use_effect effect) {
    switch(effect) {
    case use_effect::change_health: {
        return "Здоровье %1";
    }
    case use_effect::change_energy: {
        return "Энергия %1";
    }
    case use_effect::none: {
        return "-";
    }
    default: return "NULL";
    }
}
