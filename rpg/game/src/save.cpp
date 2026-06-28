#include "../header/save.h"
#include "../header/global.h"
#include "../inc/alias.h"
#include "../header/visuals.h"
#include <fstream>

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
