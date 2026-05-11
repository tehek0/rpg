#pragma once
#include "../rpg/game/inc/json.hpp"
#include "datatypes.hpp"
#include "QString"
using js = nlohmann::json;

namespace dev {
<<<<<<< Updated upstream
//Файлы с перечнем всех ныне занятых айди. Каждый файл отвечает за айди объектов своего типа данных
//Существует для проверки занят ли айди (чтобы не лезть в распаковку основных файлов с json)
const std::string paths_to_ids[amount_of_datatypes] = {
    "../../../data/location_ids.txt",
    "./../../data/sublocation_ids.txt",
    "../../../data/quest_ids.txt",
    "../../../data/enemy_ids.txt",
    "../../../data/trader_ids.txt",
    "../../../data/npc_ids.txt",
    "../../../data/disp_ids.txt",
    "../../../data/anim_disp_ids.txt",
    "../../../data/item_ids.txt",
};

//Записывает айди id в файл со всеми айди для типа данных type
void add_id_to_total(dev::datatype type, std::string id);
//Удаляет айди id из файла со всеми айди для типа данных type
void remove_id_from_total(dev::datatype type, std::string id);
//Выдаёт незанятый для данного типа данных идентефикатор
const std::string generate_id(dev::datatype type);

=======
void set_ids_default_state();
unsigned long long throw_id(datatype type);
void remove_id(datatype type, unsigned long long id);
>>>>>>> Stashed changes
}
