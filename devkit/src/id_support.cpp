#include "../header/id_support.hpp"
#include "fstream"
#include <QDebug>
#include <QFileDialog>

std::string path_to_last_ids = "../../../data/last_id.json";
std::string path_to_enemy = "../../../data/enemy.json";

//Величина отступов в json
short js_indent = 2;


namespace {
// js datatypes ={
//     {dev::datatype::location, "location"},
//     {dev::datatype::sublocation, "sublocation"},
//     {dev::datatype::item, "item"},
//     {dev::datatype::quest, "quest"},
//     {dev::datatype::enemy, "enemy"},
//     {dev::datatype::trader, "trader"},
//     {dev::datatype::npc, "npc"},
//     {dev::datatype::disp, "disp"},
//     {dev::datatype::anim_disp, "anim_disp"},
//     };



}

//Читает и записывает в файл со всеми текущими
void dev::add_to_total(dev::datatype type) {
    std::string str_type = datatypes_to_string[datatype::item];
    std::ifstream file(path_to_last_ids);
    js data = js::object();
    short current_value;

    if (!file.is_open()) {
        qInfo() << "file isn't open";
    }
    else {
        try {
            data = js::parse(file);
            current_value = data[type].get<short>();
        }
        catch (...) {
            current_value = 0;
        }

        file.close();

        std::ofstream file(path_to_last_ids);
        data[type] = current_value + 1;
        file << data.dump(js_indent);

        file.close();

        //qInfo() << data[type].get<short>();
    }
}

// Первый байт всегда зависит от типа объекта
const QString dev::generate_id(dev::datatype type) {
    std::string str_type = datatypes_to_string[type];
    std::fstream file(path_to_last_ids);
    short type_byte = type;

    js all_data = js::parse(file);
    js data = all_data[type];


    QString id = QString("%1%2").arg(type_byte).arg( + 1000);

    add_to_total(type);
    file << data.dump(js_indent);

    qInfo() << id;
    file.close();
    return id;
}

void dev::create_enemy(dev::datatype type) {
    std::ofstream file(path_to_enemy, std::ios::app);


}
