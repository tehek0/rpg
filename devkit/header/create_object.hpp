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
enum class ammo_type {
    none,
    pistol,
    shotgun,
    rifle,
    energy,
    energy_shotgun,
    rocket
};
enum class damage_type {
    bullet,
    energy,
    melee,
    unarmed,
    explosive
};
struct object_data{
    QStringList keys;
    QStringList values;
    std::vector<default_types> types;
};
void create_object(dev::datatype o_type, object_data values);

}
