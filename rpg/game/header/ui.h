#pragma once
#include "visuals.h"
#include "scenes.h"

enum class save_scene_context {
    save,
    load
};

game_scene* throw_menu_scene();
game_scene* throw_hub_scene();
game_scene* throw_dialogue_scene();
game_scene* throw_location_scene();
game_scene* throw_battle_scene();
disposable_scene* throw_settings_scene();
disposable_scene* throw_play_scene();
disposable_scene* throw_select_save_scene(save_scene_context context = save_scene_context::load);
