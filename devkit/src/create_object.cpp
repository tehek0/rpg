#include "../header/create_object.hpp"
#include "../header/inc/json.hpp"
#include "../header/id_support.hpp"
#include "../header/data/general.hpp"
#include "fstream"
#include "QRegularExpression"
#include <filesystem>
#include <QDebug>

// dev::object_data dev::object_data::operator+(const dev::object_data& other) {
//     std::vector<dev::datatype> ttypes = types_;
//     for (const auto& type: other.types_) {
//         ttypes.emplace_back(type);
//     }
//     return dev::object_data(keys_ + other.keys_, values_ + other.values_, ttypes);
// };

const QRegularExpression cut_id_out_of_line_e("[1-9][0-9]{0,}");
void dev::create_object(dev::datatype object_type, object_data& values) {
    js data;
    for (size_t i = 0; i < values.keys_.size(); ++i) {
        auto key = values.keys_[i].toStdString();
        auto value = values.values_[i];
        auto type = values.types_[i];
        if (type == dev::datatype::boolean) {
            data[key] = static_cast<bool>(value.toInt());
        }
        else if (dev::is_type_struct(type)) {
            js::array_t array;
            for (QRegularExpressionMatch elem : cut_id_out_of_line_e.globalMatch(value)) {
                array.emplace_back(elem.captured().toStdString());
            }
            data[key] = array;
        }
        else if(dev::is_type_linear(type)) {
            bool right;
            switch(type) {
            case dev::datatype::string:
                data[key] = value.toStdString(); break;
            case dev::datatype::qstring:
                data[key] = value.toStdString(); break;
            case dev::datatype::integer:
                if (value.toInt(&right)) {
                    data[key] = value.toInt();
                }
                else {
                    data[key] = INT_MAX;
                }
                break;
            case dev::datatype::u_integer:
                if (value.toUInt(&right)) {
                    data[key] = value.toUInt();
                }
                else {
                    data[key] = UINT_MAX;
                }
                break;
            case dev::datatype::short_t:
                if (value.toShort(&right)) {
                    data[key] = value.toShort();
                }
                else {
                    data[key] = SHRT_MAX;
                }
                break;
            case dev::datatype::double_t:
                data[key] = value.toDouble(); break;
            case dev::datatype::u_long_long:
                data[key] = value.toULongLong(); break;
            default: data[key] = value.toShort(); break;
            }
        }
        else {
            if (type == dev::datatype::TODO) {
                qInfo() << "[WARN][dev::create_object] function called with TO DO (unhandled) marked type. Make sure to add proper type support later";
            }

            data[key] = value.toShort();
        }
    }

    unsigned long long id = dev::throw_id(object_type);

    QString path = "objects/";
    if (is_type_component(object_type)) {
        path += "components";
    }
    path += "/%1";
    QString folder_path = QString(path).arg(datatypes_to_string[object_type]);
    if (!std::filesystem::exists(folder_path.toStdString())) {
        std::filesystem::create_directories(folder_path.toStdString());
    }
    QString file_path = (folder_path + QString("/%1_%2.json").arg(datatypes_to_string[object_type]).arg(id));
    std::ofstream file(file_path.toStdString());
    file << data.dump(js_indent);
}





