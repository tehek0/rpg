#include "../header/create_object.hpp"
#include "../rpg/game/inc/json.hpp"
#include "../header/id_support.hpp"
#include "../header/data/general.hpp"
#include "fstream"
#include <filesystem>
#include <QDebug>

dev::object_data dev::object_data::operator+(const dev::object_data& other) {
    std::vector<dev::default_types> ttypes = types_;
    for (const auto& type: other.types_) {
        ttypes.emplace_back(type);
    }
    return dev::object_data(keys_ + other.keys_, values_ + other.values_, ttypes);
};

void dev::create_object(dev::datatype object_type, object_data& values) {
    js data;
    for (size_t i = 0; i < values.keys_.size(); ++i) {
        auto key = values.keys_[i].toStdString();
        auto value = values.values_[i];
        auto type = values.types_[i];
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
        if (type == dev::default_types::TODO) {
            qInfo() << "[WARN][dev::create_object] function called with TO DO (unhandled) marked type. Make sure to add proper type support later";
            data[key] = value.toShort(); continue;
        }
        else {
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

bool dev::is_type_linear(int default_type_index) {
    return (default_type_index <= 8 && default_type_index != 0);
};
