#pragma once
#include <QStringList>
#include "../header/datatypes.hpp"
#include <vector>
namespace dev {
enum default_types {
    boolean,
    string,
    qstring,
    integer,
    short_t,
    double_t,
    u_short,
    u_integer,
    u_long_long,
    ammo_type,
    damage_type,
    item_subtypes,
    TODO,
};
struct object_data{
    QStringList keys_;
    QStringList values_;
    std::vector<default_types> types_;

    object_data() = default;
    object_data(QStringList keys, QStringList values, std::vector<default_types> types) : keys_(keys), values_(values), types_(types) {};
    object_data operator+(const object_data& other);
};

void create_object(dev::datatype object_type, object_data& values);

// Линейным считается тип, не требующий для своей записи выпадающего списка (это int, i_int, string и тд. Всё, что не bool и не кастомное)
bool is_type_linear(int default_type_index);

}
