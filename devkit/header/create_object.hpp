#pragma once
#include <QStringList>
#include "../header/datatypes.hpp"
#include <vector>
namespace dev {
enum class default_types {
    string,
    qstring,
    integer,
    short_t,
    double_t,
    boolean,
    u_integer,
    u_long_long,
    ammo_type,
    damage_type,
};
struct object_data{
    QStringList keys;
    QStringList values;
    std::vector<default_types> types;
};
void create_object(dev::datatype object_type, object_data values);

}
