#pragma once
#include <QStringList>
#include "../header/data/datatypes.hpp"
#include <vector>
namespace dev {
//Шаблон хранения данных объекта. Хранит ключи (название полей), значения и типы данны этих значений. Заготовки для типов без значений смотреть в object_dialog_templates
struct object_data{
    QStringList keys_;
    QStringList values_;
    std::vector<datatype> types_;

    object_data() = default;
    object_data(QStringList keys, QStringList values, std::vector<datatype> types) : keys_(keys), values_(values), types_(types) {};
    object_data operator+(const object_data& other);
};

//Записывает данные объекта в json
void create_object(dev::datatype object_type, object_data& values);
}
