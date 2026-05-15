#include "../header/create_object.hpp"
#include "../rpg/game/inc/json.hpp"
#include "../header/id_support.hpp"
#include "../header/general.hpp"
#include "fstream"
#include <filesystem>
//using js = nlohmann::ordered_json;

// enum class ammo_type {
//     none,
//     pistol,
//     shotgun,
//     rifle,
//     energy,
//     energy_shotgun,
//     rocket
// };
// enum class damage_type {
//     bullet,
//     energy,
//     melee,
//     unarmed,
//     explosive
// };


 void dev::create_object(dev::datatype object_type, object_data values) {
    js data;
    for (size_t i = 0; i < values.keys.size(); ++i) {
        auto key = values.keys[i].toStdString();
        auto value = values.values[i];
        auto type = values.types[i];
        if (type == dev::default_types::string || type == dev::default_types::qstring) {
            data[key] = value.toStdString(); continue;
        }
        if (type == dev::default_types::integer) {
            data[key] = value.toInt(); continue;
        }
        if (type == dev::default_types::u_integer) {
            data[key] = value.toUInt(); continue;
        }
        if (type == dev::default_types::short_t) {
            data[key] = value.toShort(); continue;
        }
        if (type == dev::default_types::double_t) {
            data[key] = value.toDouble(); continue;
        }
        if (type == dev::default_types::u_long_long) {
            data[key] = value.toULongLong(); continue;
        }
        if (type == dev::default_types::boolean) {
            data[key] = static_cast<bool>(value.toInt()); continue;
        }
        if (type == dev::default_types::damage_type || type == dev::default_types::ammo_type) {
            data[key] = value.toShort(); continue;
        }
    }

    unsigned long long id = dev::throw_id(object_type);

    QString folder_path = QString("objects/%1").arg(datatypes_to_string[object_type]);
    std::filesystem::create_directories(folder_path.toStdString());
    QString file_path = (folder_path + QString("/%1_%2.json").arg(datatypes_to_string[object_type]).arg(id));
    std::ofstream file(file_path.toStdString());

    file << data.dump(js_indent);

 };
