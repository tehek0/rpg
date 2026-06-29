#include "../header/save.h"
#include "../header/global.h"
#include "../inc/alias.h"
#include "../header/visuals.h"
#include <fstream>
#include <QMessageBox>

enum class item_types {
    none,
    weapon,
    ammo,
    armor,
    consumable
};

enum class requirement_types {
    char_requirement,
    skill_requirement
};

void critical_error(const QString& message) {
    global::w.close();
    QMessageBox::critical(nullptr, "Ошибка", message);
}

void saves_init(int amount_of_saves) {
    QString parent_dir = QString("saves");
    QString pattern = QString("slot%1");
    int saves_count = amount_of_saves;
    for (int i = 1; i <= saves_count; ++i) {
        auto root = global::root_path;
        root /= parent_dir.toStdString();
        root /= pattern.arg(i).toStdString();
        if (!std::filesystem::exists(root)) {
            std::filesystem::create_directories(root);
        }
    }
}

void save_map(map_widget* map, int slot) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot).toStdString();
    path /= "map.json";
    std::ofstream file(path.string());
    file.clear();
    json j = json::object();
    j["player_x"] = map->player_object->_disp->pos().x();
    j["player_y"] = map->player_object->_disp->pos().y();
    j["map"];
    json map_j = json::array();
    for (int i = 0; i < map->grid->tiles.size(); ++i) {
        json tile = json::object();
        map_grid_tile* tile_obj = map->grid->tiles[i];
        tile["locked"] = tile_obj->get_locked();
        if (!(tile_obj->get_poi() == nullptr)) {
            json poi = json::object();
            poi["name"] = tile_obj->get_poi()->get_name().toStdString();
            poi["location_id"] = tile_obj->get_poi()->get_location_id();
            poi["sprite"] = tile_obj->get_poi()->get_sprite_family().toStdString();
            tile["poi"] = poi;
        }
        tile["difficulty"] = tile_obj->get_difficulty();
        tile["biome"] = static_cast<int>(tile_obj->get_biome());
        map_j.emplace_back(tile);
    }
    j["map"] = map_j;
    file << j.dump(4);
}

bool load_map(map_widget* map, int slot) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot).toStdString();
    path /= "map.json";
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return false;
    }
    if (!j.is_object()) {
        return false;
    }
    json map_j;
    int player_x;
    int player_y;
    if (j.contains("player_x")) {
        j["player_x"].get_to(player_x);
    } else {
        return false;
    }
    if (j.contains("player_y")) {
        j["player_y"].get_to(player_y);
    } else {
        return false;
    }
    map->player_object->interrupt();
    map->player_object->_disp->move(player_x, player_y);
    map->player_object->move_player_marker();
    if (j.contains("map")) {
        map_j = j["map"];
    } else {
        return false;
    }
    if (!map_j.is_array()) {
        return false;
    }
    if (map_j.size() != map->grid->tiles.size()) {
        return false;
    }
    for (int i = 0; i < map->grid->tiles.size(); ++i) {
        map_grid_tile* tile_obj = map->grid->tiles[i];
        json tile = map_j[i];
        if (!tile.contains("locked") || !tile.contains("difficulty") || !tile.contains("biome")) {
            return false;
        }
        if (!tile["locked"].is_boolean() || !tile["difficulty"].is_number_float() || !tile["biome"].is_number_unsigned()) {
            return false;
        }
        bool locked;
        int biome_;
        float difficulty;
        tile["locked"].get_to(locked);
        tile["biome"].get_to(biome_);
        tile["difficulty"].get_to(difficulty);
        if (!locked) {
            tile_obj->unlock();
        } else {
            tile_obj->lock();
        }
        tile_obj->set_biome(biome{biome_});
        tile_obj->set_difficulty(difficulty);
        if (tile.contains("poi")) {
            json tile_poi = tile["poi"];

            if (!tile_poi.is_object())
                return false;

            if (!tile_poi.contains("sprite") || !tile_poi.contains("name") || !tile_poi.contains("location_id"))
                return false;

            if (!tile_poi["sprite"].is_string() || !tile_poi["name"].is_string() || !tile_poi["location_id"].is_number_unsigned())
                return false;

            std::string sprite;
            std::string name;
            unsigned long long location_id;

            tile_poi["sprite"].get_to(sprite);
            tile_poi["name"].get_to(name);
            tile_poi["location_id"].get_to(location_id);

            map_poi* poi = new map_poi(QString::fromStdString(sprite), location_id, QString::fromStdString(name));
            tile_obj->set_poi(poi);
        }
    }
    return true;
}

void save_player(player* player_, int slot) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot).toStdString();
    path /= "player.json";
    std::ofstream file(path.string());
    file.clear();
    json j = json::object();
    j["name"] = player_->get_name().toStdString();
    j["asset"] = player_->get_asset().toStdString();
    j["entity_stats"] = json_from_entity_stats(player_->get_entity_stats());
    j["entity_level"] = json_from_entity_level(player_->get_entity_level());
    j["max_health"] = player_->get_max_health();
    j["health"] = player_->get_health();
    j["base_armor"] = player_->get_base_armor();
    j["money"] = player_->get_money();
    j["max_weight"] = player_->get_max_weight();
    j["max_energy"] = player_->get_max_energy();
    json inv = json::array();
    for (auto elem : player_->get_inventory()->get_items()) {
        json item_ = json_from_item(elem);
        if (player_->get_inventory()->is_equipped(elem)) {
            item_["is_equipped"] = true;
        }
        inv.emplace_back(item_);
    }
    j["inventory"] = inv;
    file << j.dump(4);
}

bool load_player(player* player_, int slot) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot).toStdString();
    path /= "player.json";
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return false;
    }
    if (!j.is_object()) {
        return false;
    }
    if (!j.contains("name") || !j.contains("asset") || !j.contains("entity_stats") || !j.contains("entity_level") || !j.contains("max_health") || !j.contains("health") || !j.contains("base_armor") || !j.contains("money") || !j.contains("max_weight") || !j.contains("max_energy") || !j.contains("inventory"))
        return false;
    if (!j["name"].is_string() || !j["asset"].is_string() || !j["entity_stats"].is_object() || !j["entity_level"].is_object() || !j["max_health"].is_number_unsigned() || !j["health"].is_number_unsigned() || !j["base_armor"].is_number_integer() || !j["money"].is_number_unsigned() || !j["max_weight"].is_number_float() || !j["max_energy"].is_number_integer() || !j["inventory"].is_array())
        return false;

    std::string _name;
    std::string _asset;
    entity_stats _entity_stats = entity_stats_from_json(j["entity_stats"]);
    entity_level _entity_level = entity_level_from_json(j["entity_level"]);
    unsigned int _max_health;
    unsigned int _health;
    int _base_armor;
    int _money;
    float _max_weight;
    int _max_energy;
    inventory* _inventory;
    j["name"].get_to(_name);
    j["asset"].get_to(_asset);
    j["max_health"].get_to(_max_health);
    j["health"].get_to(_health);
    j["base_armor"].get_to(_base_armor);
    j["money"].get_to(_money);
    j["max_weight"].get_to(_max_weight);
    j["max_energy"].get_to(_max_energy);
    _inventory = new inventory();
    for (auto elem : j["inventory"]) {
        item* itm;
        if (elem.is_object()) {
            itm = item_from_json(elem);
        } else if (elem.is_number_unsigned()) {
            unsigned long long id;
            elem.get_to(id);
            itm = item_from_id(id);
        } else {
            delete _inventory;
            return false;
        }
        if (itm == nullptr) {
            delete _inventory;
            return false;
        }
        _inventory->add_item(itm);
    }
    if (player_ == nullptr)
        player_ = new player();

    player_->set_name(QString::fromStdString(_name));
    player_->set_asset(QString::fromStdString(_asset));
    player_->set_entity_stats(_entity_stats);
    player_->set_entity_level(_entity_level);
    player_->set_max_health(_max_health);
    player_->set_health(_health);
    player_->set_base_armor(_base_armor);
    player_->set_money(_money);
    player_->set_max_weight(_max_weight);
    player_->set_max_energy(_max_energy);
    player_->set_inventory(_inventory);
    return true;
}


json json_from_entity_stats(entity_stats stats) {
    json j = json::object();

    j["strength"] = stats.strength;
    j["agility"] = stats.agility;
    j["endurance"] = stats.endurance;
    j["intelligence"] = stats.intelligence;
    j["luck"] = stats.luck;

    j["guns"] = stats.guns;
    j["big_guns"] = stats.big_guns;
    j["unarmed"] = stats.unarmed;
    j["science"] = stats.science;
    j["speech"] = stats.speech;
    j["barter"] = stats.barter;
    j["survival"] = stats.survival;

    return j;
}
entity_stats entity_stats_from_json(json j) {
    if (!j.is_object()) {
        critical_error("Не удалось загрузить entity_stats.");
        return entity_stats();
    }
    if (!j.contains("strength") || !j.contains("agility") || !j.contains("endurance") || !j.contains("intelligence") || !j.contains("luck") || !j.contains("guns") || !j.contains("big_guns") || !j.contains("unarmed") || !j.contains("science") || !j.contains("speech") || !j.contains("barter") || !j.contains("survival")) {
        critical_error("Не удалось загрузить entity_stats.");
        return entity_stats();
    }
    if (!j["strength"].is_number_integer() || !j["agility"].is_number_integer() || !j["endurance"].is_number_integer() || !j["intelligence"].is_number_integer() || !j["luck"].is_number_integer() || !j["guns"].is_number_integer() || !j["big_guns"].is_number_integer() || !j["unarmed"].is_number_integer() || !j["science"].is_number_integer() || !j["speech"].is_number_integer() || !j["barter"].is_number_integer() || !j["survival"].is_number_integer()) {
        critical_error("Не удалось загрузить entity_stats.");
        return entity_stats();
    }
    entity_stats stats;
    j["strength"].get_to(stats.strength);
    j["agility"].get_to(stats.agility);
    j["endurance"].get_to(stats.endurance);
    j["intelligence"].get_to(stats.intelligence);
    j["luck"].get_to(stats.luck);
    j["guns"].get_to(stats.guns);
    j["big_guns"].get_to(stats.big_guns);
    j["unarmed"].get_to(stats.unarmed);
    j["science"].get_to(stats.science);
    j["speech"].get_to(stats.speech);
    j["barter"].get_to(stats.barter);
    j["survival"].get_to(stats.survival);
    return stats;
}

json json_from_entity_level(entity_level _entity_level) {

    json level = json::object();
    level["level"] = _entity_level.level;
    level["experience"] = _entity_level.experiecne;
    level["scaling"] = _entity_level.scaling;

    return level;
}
entity_level entity_level_from_json(json j) {
    if (!j.contains("level") || !j.contains("experience") || !j.contains("scaling")) {
        critical_error("Не удалось загрузить entity_level.");
        return entity_level();
    }
    if (!j["level"].is_number_unsigned() || !j["experience"].is_number_unsigned() || !j["scaling"].is_number_unsigned()) {
        critical_error("Не удалось загрузить entity_level.");
        return entity_level();
    }
    entity_level _level;
    j["level"].get_to(_level.level);
    j["experience"].get_to(_level.experiecne);
    j["scaling"].get_to(_level.scaling);
    return _level;
}

json json_from_item(item* item_) {
    json j = json::object();
    j["subtype"] = static_cast<int>(item_types::none);
    j["name"] = item_->get_name().toStdString();
    j["desc"] = item_->get_desc().toStdString();
    j["asset"] = item_->get_asset().toStdString();
    j["stack"] = item_->get_stack();
    j["max_stack_size"] = item_->get_max_stack_size();
    j["base_weight"] = item_->get_base_weight();
    j["base_cost"] = item_->get_base_cost();
    j["sellable"] = item_->get_sellable();
    if (item_->is_item_with_requirements()) {
        add_requirements_keys(j, static_cast<item_with_requirements*>(item_)->get_item_requirements());
    }
    if (item_->is_ammo_type()) {
        add_ammo_keys(j, static_cast<ammo*>(item_));
    }
    if (item_->is_armor_type()) {
        add_armor_keys(j, static_cast<armor*>(item_));
    }
    if (item_->is_consumable_type()) {
        add_consumable_keys(j, static_cast<consumable*>(item_));
    }
    if (item_->is_weapon_type()) {
        add_weapon_keys(j, static_cast<weapon*>(item_));
    }
    return j;
}

void add_requirements_keys(json &j, item_requirements *requirements) {
    json ir_o = json::object();
    ir_o["min_level"] = requirements->min_level;
    json ptrs = json::array();
    for (auto elem : requirements->item_requirements_ptrs) {
        json require = json::object();
        if (elem->is_char_requirement()) {
            require["subtype"] = static_cast<int>(requirement_types::char_requirement);
            require["type"] = static_cast<int>(static_cast<char_requirement*>(elem)->type);
        } else {
            require["subtype"] = static_cast<int>(requirement_types::skill_requirement);
            require["type"] = static_cast<int>(static_cast<skill_requirement*>(elem)->type);
        }
        require["required"] = elem->required;
        ptrs.emplace_back(require);
    }
    ir_o["requirements"] = ptrs;
    j["item_requirement"] = ir_o;
}

void add_ammo_keys(json &j, ammo *ammo_) {
    j["subtype"] = static_cast<int>(item_types::ammo);
    j["base_dmg"] = ammo_->get_base_dmg();
    j["ammo_type"] = static_cast<int>(ammo_->get_ammo_type());
}

void add_armor_keys(json &j, armor *armor_) {
    j["subtype"] = static_cast<int>(item_types::armor);
    json bonus = json::object();
    bonus["equipment_bonus"] = static_cast<int>(armor_->get_armor_bonus().bonus);
    bonus["value"] = armor_->get_armor_bonus().value;
    j["armor_slot"] = static_cast<int>(armor_->get_armor_slot());
    j["armor_points"] = armor_->get_armor_points();
    j["armor_bonus"] = bonus;
}

void add_consumable_keys(json &j, consumable *consumable_) {
    j["subtype"] = static_cast<int>(item_types::consumable);
    json use = json::object();
    use["effect"] = static_cast<int>(consumable_->get_on_use().effect);
    use["value"] = consumable_->get_on_use().value;
    j["energy_cost"] = consumable_->get_energy_cost();
    j["on_use"] = use;
    j["uses_left"] = consumable_->get_uses_left();
}

void add_weapon_keys(json &j, weapon *weapon_) {
    j["subtype"] = static_cast<int>(item_types::weapon);
    j["base_dmg"] = weapon_->get_base_dmg();
    j["ammo_type"] = static_cast<int>(weapon_->get_ammo_type());
    j["energy_cost"] = weapon_->get_energy_cost();
    j["damage_type"] = static_cast<int>(weapon_->get_damage_type());
}

item* item_from_json(json j) {
    int subtype;
    std::string _name;
    std::string _desc;
    std::string _asset;
    unsigned int _stack;
    unsigned int _max_stack_size;
    double _base_weight;
    unsigned int _base_cost;
    bool _sellable;
    if (!j.contains("name") || !j.contains("desc") || !j.contains("asset") || !j.contains("stack") || !j.contains("max_stack_size") || !j.contains("base_weight") || !j.contains("base_cost") || !j.contains("sellable"))
        return nullptr;

    if (!j["name"].is_string() || !j["desc"].is_string() || !j["asset"].is_string() || !j["stack"].is_number_unsigned() || !j["max_stack_size"].is_number_unsigned() || !j["base_weight"].is_number_float() || !j["base_cost"].is_number_unsigned() || !j["sellable"].is_boolean())
        return nullptr;

    j["name"].get_to(_name);
    j["desc"].get_to(_desc);
    j["asset"].get_to(_asset);
    j["stack"].get_to(_stack);
    j["max_stack_size"].get_to(_max_stack_size);
    j["base_weight"].get_to(_base_weight);
    j["base_cost"].get_to(_base_cost);
    j["sellable"].get_to(_sellable);

    if (j.contains("subtype")) {
        if (j["subtype"].is_number_unsigned()) {
            j["subtype"].get_to(subtype);
            if (subtype == 0) {
                return new item(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable);
            }
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }

    item_types subtype_ = item_types{subtype};
    if (subtype_ == item_types::ammo) {
        if (!j.contains("base_dmg") || !j.contains("ammo_type"))
            return nullptr;

        if (!j["base_dmg"].is_number_integer() || !j["ammo_type"].is_number_unsigned())
            return nullptr;

        int base_dmg;
        int ammo_type_;
        j["base_dmg"].get_to(base_dmg);
        j["ammo_type"].get_to(ammo_type_);
        return new ammo(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable, base_dmg, ammo_type{ammo_type_});
    } else if (subtype_ == item_types::consumable) {
        if (!j.contains("on_use") || !j.contains("uses_left") || !j.contains("energy_cost"))
            return nullptr;
        if (!j["uses_left"].is_number_unsigned() || !j["energy_cost"].is_number_unsigned())
            return nullptr;
        on_use on_use_;
        if (j["on_use"].is_object()) {
            on_use_ = on_use_from_json(j["on_use"]);
        } else if (j["on_use"].is_number_unsigned()) {
            unsigned long long id;
            j["on_use"].get_to(id);
            on_use_ = on_use_from_id(id);
        } else {
            return nullptr;
        }
        unsigned short _uses_left;
        short _energy_cost;
        j["uses_left"].get_to(_uses_left);
        j["energy_cost"].get_to(_energy_cost);
        return new consumable(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable, on_use_, _uses_left, _energy_cost);
    } else if (subtype_ == item_types::weapon) {
        if (!j.contains("base_dmg") || !j.contains("ammo_type") || !j.contains("damage_type") || !j.contains("energy_cost"))
            return nullptr;
        if (!j["base_dmg"].is_number_integer() || !j["ammo_type"].is_number_unsigned() || !j["damage_type"].is_number_unsigned() || !j["energy_cost"].is_number_unsigned())
            return nullptr;


        int _base_dmg;
        int _ammo_type;
        int _damage_type;
        short _energy_cost;
        item_requirements* requirements;
        j["base_dmg"].get_to(_base_dmg);
        j["ammo_type"].get_to(_ammo_type);
        j["damage_type"].get_to(_damage_type);
        j["energy_cost"].get_to(_energy_cost);
        if (j.contains("item_requirement")) {
            json req = j["item_requirement"];
            if (req.is_object()) {
                requirements = requirements_from_json(req);
            } else if (req.is_number_unsigned()) {
                unsigned long long id;
                req.get_to(id);
                requirements = requirements_from_id(id);
            } else {
                return nullptr;
            }
            if (requirements == nullptr) {
                return nullptr;
            }
        }
        return new weapon(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable, requirements, _base_dmg, damage_type{_damage_type}, ammo_type{_ammo_type}, _energy_cost);
    } else if (subtype_ == item_types::armor) {
        if (!j.contains("armor_slot") || !j.contains("armor_points") || !j.contains("armor_bonus"))
            return nullptr;
        if (!j["armor_slot"].is_number_unsigned() || !j["armor_points"].is_number_integer())
            return nullptr;
        armor_bonus _armor_bonus;
        if (j["armor_bonus"].is_object()) {
            _armor_bonus = armor_bonus_from_json(j["armor_bonus"]);
        } else if (j["armor_bonus"].is_number_unsigned()) {
            unsigned long long id;
            j["armor_bonus"].get_to(id);
            _armor_bonus = armor_bonus_from_id(id);
        } else {
            return nullptr;
        };
        int _armor_slot;
        short _armor_points;
        j["armor_slot"].get_to(_armor_slot);
        j["armor_points"].get_to(_armor_points);
        item_requirements* requirements;
        if (j.contains("item_requirement")) {
            json req = j["item_requirement"];
            if (req.is_object()) {
                requirements = requirements_from_json(req);
            } else if (req.is_number_unsigned()) {
                unsigned long long id;
                req.get_to(id);
                requirements = requirements_from_id(id);
            } else {
                return nullptr;
            }
            if (requirements == nullptr) {
                return nullptr;
            }
        }
        return new armor(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable, requirements, armor_slot{_armor_slot}, _armor_points, _armor_bonus);
    } else {
        return nullptr;
    }
}

item* item_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "item";
    path /= QString("item_%1.json").arg(id).toStdString();
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return nullptr;
    }
    if (!j.is_object()) {
        return nullptr;
    }
    return item_from_json(j);
}

item_requirements* requirements_from_json(json j) {
    if (!j.is_object())
        return nullptr;
    if (j.empty()) {
        return new item_requirements();
    }
    if (!j.contains("min_level") || !j.contains("requirements"))
        return nullptr;

    if (!j["min_level"].is_number_integer() || !j["requirements"].is_array())
        return nullptr;
    int _min_level;
    j["min_level"].get_to(_min_level);
    item_requirements* req = new item_requirements();
    req->min_level = _min_level;
    req->item_requirements_ptrs.reserve(j["requirements"].size());
    for (auto elem : j["requirements"]) {
        if (elem.is_object()) {
            base_requirement* requ = single_requirement_from_json(elem);
            if (requ == nullptr) {
                delete req;
                return nullptr;
            }
            req->item_requirements_ptrs.emplace_back(requ);
        } else if (elem.is_number_unsigned()) {
            unsigned long long id;
            elem.get_to(id);
            base_requirement* requ = single_requirement_from_id(id);
            if (requ == nullptr) {
                delete req;
                return nullptr;
            }
            req->item_requirements_ptrs.emplace_back(requ);
        } else {
            delete req;
            return nullptr;
        }
    }
    return req;
}

item_requirements* requirements_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "item_requirement";
    path /= QString("item_requirement_%1.json").arg(id).toStdString();
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return nullptr;
    }
    if (!j.is_object()) {
        return nullptr;
    }
    return requirements_from_json(j);
}

base_requirement* single_requirement_from_json(json j) {
    if (!j.contains("subtype") || !j.contains("type") || !j.contains("required"))
        return nullptr;
    if (!j["subtype"].is_number_unsigned() || !j["type"].is_number_unsigned() || !j["required"].is_number_integer())
        return nullptr;

    int subtype;
    int type;
    int required;
    j["subtype"].get_to(subtype);
    j["type"].get_to(type);
    j["required"].get_to(required);
    if (requirement_types{subtype} == requirement_types::char_requirement) {
        return new char_requirement(required, char_type{type});
    } else {
        return new skill_requirement(required, skill_type{type});
    }
}

base_requirement* single_requirement_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "components";
    path /= "requirement";
    path /= QString("requirement_%1.json").arg(id).toStdString();
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        return nullptr;
    }
    if (!j.is_object()) {
        return nullptr;
    }
    return single_requirement_from_json(j);
}

armor_bonus armor_bonus_from_json(json j) {
    if (!j.contains("equipment_bonus") || !j.contains("value")) {
        critical_error("Не удалось загрузить объект типа armor_bonus.");
        return armor_bonus();
    }
    if (!j["equipment_bonus"].is_number_integer() || !j["value"].is_number_integer()) {
        critical_error("Не удалось загрузить объект типа armor_bonus.");
        return armor_bonus();
    }
    int _equipment_bonus;
    int _value;
    j["equipment_bonus"].get_to(_equipment_bonus);
    j["value"].get_to(_value);
    armor_bonus bonus_;
    bonus_.bonus = equipment_bonus{_equipment_bonus};
    bonus_.value = _value;
    return bonus_;
}

armor_bonus armor_bonus_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "armor_bonus";
    path /= QString("armor_bonus_%1.json").arg(id).toStdString();
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        critical_error(QString("Не удалось загрузить объект armor_bonus_%1.json").arg(id));
        return armor_bonus();
    }
    if (!j.is_object()) {
        critical_error(QString("Не удалось загрузить объект armor_bonus_%1.json").arg(id));
        return armor_bonus();
    }
    return armor_bonus_from_json(j);
}

on_use on_use_from_json(json j) {
    if (!j.contains("effect") || !j.contains("value")) {
        critical_error("Не удалось загрузить объект типа on_use.");
        return on_use();
    }
    if (!j["effect"].is_number_integer() || !j["value"].is_number_integer()) {
        critical_error("Не удалось загрузить объект типа on_use.");
        return on_use();
    }
    int _effect;
    int _value;
    j["effect"].get_to(_effect);
    j["value"].get_to(_value);
    on_use on_use_;
    on_use_.effect = use_effect{_effect};
    on_use_.value = _value;
    return on_use_;
}

on_use on_use_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "on_use";
    path /= QString("on_use_%1.json").arg(id).toStdString();
    std::ifstream file(path.string());
    json j;
    try {
        j = json::parse(file);
    } catch (...) {
        critical_error(QString("Не удалось загрузить объект on_use_%1.json").arg(id));
        return on_use();
    }
    if (!j.is_object()) {
        critical_error(QString("Не удалось загрузить объект on_use_%1.json").arg(id));
        return on_use();
    }
    return on_use_from_json(j);
}
