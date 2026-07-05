#include "../header/save.h"
#include "../header/global.h"
#include "../inc/alias.h"
#include "../header/visuals.h"
#include <fstream>
#include <ctime>
#include "../header/ui.h"
#include <QMessageBox>



void save(int slot_num) {
    save_init(slot_num);
    save_map(global::w.hub_scene->findChild<map_widget*>("map", Qt::FindDirectChildrenOnly), slot_num);
    save_player(global::player_, slot_num);
    save_meta(slot_num);
}

void load(int slot_num) {
    delete global::w.hub_scene;
    delete global::player_;
    global::w.hub_scene = throw_hub_scene();
    global::player_ = new player();
    if (!load_player(global::player_, slot_num)) {
        critical_error(QString("Не удалось загрузить сохранение %1, данные игрока повреждены").arg(slot_num));
        return;
    }
    if (!load_map(global::w.hub_scene->findChild<map_widget*>("map", Qt::FindDirectChildrenOnly), slot_num)) {
        critical_error(QString("Не удалось загрузить сохранение %1, данные карты повреждены").arg(slot_num));
        return;
    }
    global::w.switch_to_scene(global::w.hub_scene);
    global::music->set_music("ambience");
}

void delete_save(int slot_num) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot_num).toStdString();
    std::filesystem::remove_all(path);
}




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

void save_init(int slot_num) {
    auto root = global::root_path;
    root /= "saves";
    root /= QString("slot%1").arg(slot_num).toStdString();
    if (!std::filesystem::exists(root)) {
        std::filesystem::create_directories(root);
    }
}

bool save_has_data(int slot_num) {
    auto root = global::root_path;
    root /= "saves";
    root /= QString("slot%1").arg(slot_num).toStdString();
    root /= "meta.json";
    std::ifstream file(root.string());
    if (!file.is_open()) {
        return false;
    }
    return true;
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
    if (slot > 0) {
        path /= "saves";
        path /= QString("slot%1").arg(slot).toStdString();
    } else {
        path /= "objects";
    }
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
    j["level_up_points"] = player_->get_level_up_points();
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
    unsigned int _level_up_points = 0;
    if (j.contains("level_up_points")) {
        if (j["level_up_points"].is_number_unsigned()) {
            j["level_up_points"].get_to(_level_up_points);
        }
    }
    inventory* _inventory;
    j["name"].get_to(_name);
    j["asset"].get_to(_asset);
    j["max_health"].get_to(_max_health);
    j["health"].get_to(_health);
    j["base_armor"].get_to(_base_armor);
    j["money"].get_to(_money);
    j["max_weight"].get_to(_max_weight);
    j["max_energy"].get_to(_max_energy);

    if (player_ == nullptr)
        player_ = new player();

    player_->set_name(QString::fromStdString(_name));
    player_->set_asset(QString::fromStdString(_asset));
    player_->set_entity_stats(_entity_stats);
    player_->set_max_health(_max_health);
    player_->set_health(_health);
    player_->set_base_armor(_base_armor);
    player_->set_money(_money);
    player_->set_max_weight(_max_weight);
    player_->set_max_energy(_max_energy);
    player_->set_entity_level(_entity_level);
    player_->set_level_up_points(_level_up_points);
    _inventory = new inventory();
    player_->set_inventory(_inventory);
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
        if (elem.contains("is_equipped")) {
            _inventory->equip_unconditional(itm);
        }
    }

    return true;
}

void save_meta(int slot) {
    auto path = global::root_path;
    path /= "saves";
    path /= QString("slot%1").arg(slot).toStdString();
    path /= "meta.json";
    std::ofstream file(path.string());
    file.clear();
    json j = json::object();
    time_t time_;
    std::tm* timeinfo;
    std::time(&time_);
    timeinfo = std::localtime(&time_);
    QString day_str = QString::number(timeinfo->tm_mday);
    format_time_data(day_str);
    QString month_str = QString::number(timeinfo->tm_mon + 1);
    format_time_data(month_str);
    QString year_str = QString::number(1900 + timeinfo->tm_year);
    QString hour_str = QString::number(timeinfo->tm_hour);
    format_time_data(hour_str);
    QString min_str = QString::number(timeinfo->tm_min);
    format_time_data(min_str);
    QString sec_str = QString::number(timeinfo->tm_sec);
    format_time_data(sec_str);
    QString result_time = day_str + "." + month_str + "." + year_str + ", " + hour_str + ":" + min_str + ":" + sec_str;
    j["player_name"] = global::player_->get_name().toStdString();
    j["player_level"] = global::player_->get_entity_level().level;
    j["save_time"] = result_time.toStdString();
    qInfo() << "saved at" << result_time;
    file << j.dump(4);
}

void format_time_data(QString &data) {
    if (data.size() == 1) {
        data = "0" + data;
    }
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
    level["current_needed"] = _entity_level.current_needed;
    level["scaling"] = _entity_level.scaling;

    return level;
}
entity_level entity_level_from_json(json j) {
    if (!j.contains("level") || !j.contains("experience") || !j.contains("scaling") || !j.contains("current_needed")) {
        critical_error("Не удалось загрузить entity_level.");
        return entity_level();
    }
    if (!j["level"].is_number_unsigned() || !j["experience"].is_number_unsigned() || !j["scaling"].is_number_unsigned() || !j["current_needed"].is_number_unsigned()) {
        critical_error("Не удалось загрузить entity_level.");
        return entity_level();
    }
    entity_level _level;
    j["level"].get_to(_level.level);
    j["experience"].get_to(_level.experiecne);
    j["current_needed"].get_to(_level.current_needed);
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
    bonus["bonus"] = static_cast<int>(armor_->get_armor_bonus().bonus);
    bonus["value"] = armor_->get_armor_bonus().value;
    j["armor_slot"] = static_cast<int>(armor_->get_armor_slot());
    j["armor_points"] = armor_->get_armor_points();
    j["armor_bonus"] = bonus;
}

void add_consumable_keys(json &j, consumable *consumable_) {
    j["subtype"] = static_cast<int>(item_types::consumable);
    json use = json::object();
    use["use_effects"] = static_cast<int>(consumable_->get_on_use().effect);
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
    if (!j.contains("stack")) {
        j["stack"] = 1u;
    }
    if (!j.contains("name") || !j.contains("desc") || !j.contains("asset") || !j.contains("max_stack_size") || !j.contains("base_weight") || !j.contains("base_cost") || !j.contains("sellable"))
        return nullptr;

    if (!j["name"].is_string() || !j["desc"].is_string() || !j["asset"].is_string() || !j["stack"].is_number_integer() || !j["max_stack_size"].is_number_unsigned() || !j["base_weight"].is_number_float() || !j["base_cost"].is_number_unsigned() || !j["sellable"].is_boolean())
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
        } else {
            requirements = new item_requirements();
        }
        return new weapon(QString::fromStdString(_name), QString::fromStdString(_desc), QString::fromStdString(_asset), _stack, _max_stack_size, _base_weight, _base_cost, _sellable, requirements, _base_dmg, damage_type{_damage_type}, ammo_type{_ammo_type}, _energy_cost);
    } else if (subtype_ == item_types::armor) {
        if (!j.contains("armor_slot") || !j.contains("armor_points"))
            return nullptr;
        if (!j["armor_slot"].is_number_unsigned() || !j["armor_points"].is_number_integer())
            return nullptr;
        armor_bonus _armor_bonus;
        if (j.contains("armor_bonus")) {
            if (j["armor_bonus"].is_object()) {
                _armor_bonus = armor_bonus_from_json(j["armor_bonus"]);
            } else if (j["armor_bonus"].is_number_unsigned()) {
                unsigned long long id;
                j["armor_bonus"].get_to(id);
                _armor_bonus = armor_bonus_from_id(id);
            } else {
                return nullptr;
            };
        }
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
        } else {
            requirements = new item_requirements();
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
    if (!j.contains("bonus") || !j.contains("value")) {
        critical_error("Не удалось загрузить объект типа armor_bonus.");
        return armor_bonus();
    }
    if (!j["bonus"].is_number_integer() || !j["value"].is_number_integer()) {
        critical_error("Не удалось загрузить объект типа armor_bonus.");
        return armor_bonus();
    }
    int _equipment_bonus;
    int _value;
    j["bonus"].get_to(_equipment_bonus);
    j["value"].get_to(_value);
    armor_bonus bonus_;
    bonus_.bonus = equipment_bonus{_equipment_bonus};
    bonus_.value = _value;
    return bonus_;
}

armor_bonus armor_bonus_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "components";
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
    if (!j.contains("use_effects") || !j.contains("value")) {
        critical_error("Не удалось загрузить объект типа on_use.");
        return on_use();
    }
    if (!j["use_effects"].is_number_integer() || !j["value"].is_number_integer()) {
        critical_error("Не удалось загрузить объект типа on_use.");
        return on_use();
    }
    int _effect;
    int _value;
    j["use_effects"].get_to(_effect);
    j["value"].get_to(_value);
    on_use on_use_;
    on_use_.effect = use_effect{_effect};
    on_use_.value = _value;
    return on_use_;
}

on_use on_use_from_id(unsigned long long id) {
    auto path = global::root_path;
    path /= "objects";
    path /= "components";
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

slot::slot(int slot_num, QWidget* parent): QPushButton(parent) {
    this->resize(350, 100);
    this->setCheckable(true);
    this->setStyleSheet("QPushButton { background-color: lightgray; }"
                        "QPushButton:checked { background-color: #55FF55; }");
    save_slot = new QLabel(this);
    save_slot->setGeometry(5, 0, 350, 50);
    save_slot->setStyleSheet("font: 17pt \"Arial\"");
    player_info = new QLabel(this);
    player_info->setGeometry(5, 0, 350, 75);
    player_info->setAlignment(Qt::AlignBottom);
    save_date = new QLabel(this);
    save_date->setGeometry(5, 0, 350, 95);
    save_date->setAlignment(Qt::AlignBottom);
    connect(this, &slot::clicked, this, &slot::on_click);
    set_slot(slot_num);
}
void slot::set_slot(int slot_num) {
    slot_number = slot_num;
    save_slot->setText(QString("Слот %1").arg(slot_number));
    if (save_has_data(slot_number)) {
        this->setText("");
        is_empty = false;

        auto root = global::root_path;
        root /= "saves";
        root /= QString("slot%1").arg(slot_number).toStdString();
        root /= "meta.json";
        json j;
        try {
            j = json::parse(std::ifstream(root.string()));
        } catch (...) {
            player_info->setText("Неизвестные данные игрока");
            save_date->setText("Неизвестная дата сохранения");
        }

        std::string player_name;
        int player_level;
        if (!j.contains("player_name") || !j.contains("player_level")) {
            player_info->setText("Неизвестные данные игрока");
        } else if (!j["player_name"].is_string() || !j["player_level"].is_number_unsigned()) {
            player_info->setText("Неизвестные данные игрока");
        } else {
            j["player_name"].get_to(player_name);
            j["player_level"].get_to(player_level);
            player_info->setText(QString("%1, Уровень %2").arg(player_name).arg(player_level));
        }
        std::string save_time;
        if (!j.contains("save_time")) {
            save_date->setText("Неизвестная дата сохранения");
        } else if (!j["save_time"].is_string()) {
            save_date->setText("Неизвестная дата сохранения");
        } else {
            j["save_time"].get_to(save_time);
            save_date->setText(QString::fromStdString(save_time));
        }
    } else {
        this->setText("Пустой слот");
        player_info->setText("");
        save_date->setText("");
        is_empty = true;
    }
}

void slot::on_click() {
    emit select(this);
}

void save_widget::wheelEvent(QWheelEvent *event)  {
    scrolled(event->angleDelta().y());
    event->accept();
}

void save_widget::paintEvent(QPaintEvent *event) {
    QPainter paint(this);
    if (sprite.isNull())
        return;

    paint.drawTiledPixmap(this->rect(), sprite);
}

save_widget::save_widget(QWidget *parent): QWidget(parent) {
    this->resize(360, 360);
    sprite.load(":/pictures/testbkg_save_widget.jpg");
    slot1 = new slot(1, this);
    slot1->move(5, 5);
    connect(slot1, &slot::select, this, &save_widget::select_slot);
    slot2 = new slot(2, this);
    slot2->move(5, 110);
    connect(slot2, &slot::select, this, &save_widget::select_slot);
    slot3 = new slot(3, this);
    slot3->move(5, 215);
    connect(slot3, &slot::select, this, &save_widget::select_slot);
    load_button = new QPushButton("Загрузить", this);
    load_button->setDisabled(true);
    load_button->setGeometry(125,335,110,20);
    connect(load_button, &QPushButton::clicked, this, &save_widget::load_selected);
    save_button = new QPushButton("Сохранить", this);
    save_button->setDisabled(true);
    save_button->setGeometry(10, 335, 110, 20);
    connect(save_button, &QPushButton::clicked, this, &save_widget::save_selected);
    delete_button = new QPushButton("Удалить", this);
    delete_button->setDisabled(true);
    delete_button->setGeometry(240, 335, 110, 20);
    connect(delete_button, &QPushButton::clicked, this, &save_widget::delete_selected);
}

void save_widget::scrolled(int delta) {
    if (delta > 0) {
        if (scrolled_past > 0) {
            scrolled_past -= 3;
        }
    } else {
        scrolled_past += 3;
    }
    slot1->set_slot(1 + scrolled_past);
    slot2->set_slot(2 + scrolled_past);
    slot3->set_slot(3 + scrolled_past);
    deselect_slot();
}

void save_widget::select_slot(slot *slot_) {
    deselect_slot();
    selected_slot = slot_;
    slot_->setChecked(true);
    save_button->setDisabled(is_save_perma_locked);
    if (!selected_slot->is_empty) {
        load_button->setDisabled(is_load_perma_locked);
        delete_button->setDisabled(is_delete_perma_locked);
    }
}

void save_widget::load_selected() {
    if (selected_slot == nullptr)
        return;

    load(selected_slot->slot_number);
}

void save_widget::save_selected() {
    if (selected_slot == nullptr)
        return;

    save(selected_slot->slot_number);
    selected_slot->set_slot(selected_slot->slot_number);
    select_slot(selected_slot);
}

void save_widget::delete_selected() {
    if (selected_slot == nullptr)
        return;

    delete_save(selected_slot->slot_number);
    selected_slot->set_slot(selected_slot->slot_number);
    deselect_slot();
}

void save_widget::deselect_slot() {
    if (selected_slot == nullptr)
        return;
    selected_slot->setChecked(false);
    save_button->setDisabled(true);
    load_button->setDisabled(true);
    delete_button->setDisabled(true);
    selected_slot = nullptr;
}
