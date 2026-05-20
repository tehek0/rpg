#pragma once
#include <QStringList>
#include "../header/data/datatypes.hpp"
#include <vector>
namespace dev {
//Шаблон хранения данных объекта. Хранит ключи (название полей), значения и типы данны этих значений. Заготовки для типов без значений смотреть в object_dialog_templates
struct object_data{
    QStringList keys_;
    QStringList values_;
    std::vector<default_types> types_;

    object_data() = default;
    object_data(QStringList keys, QStringList values, std::vector<default_types> types) : keys_(keys), values_(values), types_(types) {};
    object_data operator+(const object_data& other);
};

//Записывает данные объекта в json
void create_object(dev::datatype object_type, object_data& values);

// Проверк линейности. Линейным считается тип, не требующий для своей записи выпадающего списка (это int, i_int, string и тд. Всё, что не bool и не кастомное)
bool is_type_linear(int default_type_index);
bool is_type_struct(int default_type_index);
}
