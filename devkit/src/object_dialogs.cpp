#include "../header/object_dialogs.hpp"
#include <QDebug>
#include <QComboBox>


const short label_w = 120;
const short field_w = 300;
const short any_line_hight = 25;
const short gap = 5;

dev::object_data templates[dev::datatype::total + 1] = {
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {
     {"subtype", "name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
     {},
     {dev::default_types::item_subtypes, dev::default_types::qstring, dev::default_types::qstring, dev::default_types::qstring,
         dev::default_types::u_integer, dev::default_types::u_integer,dev::default_types::u_integer, dev::default_types::boolean},
    },
    {},
};

const std::vector<dev::object_data> subtypes_template[dev::datatype::total] = {
    { //location's
        {{},{},{},},
    },
    { //sublocation's
        {{},{},{},},
     },
    { //etc
        {{},{},{},},
     },
    {
        {{},{},{},},
     },
    {
        {{},{},{},},
     },
    {
        {{},{},{},},
     },
    {
        {{},{},{},},
     },
    {
        {{},{},{},},
     },
    {// item's
        {//none},
        { //weapon
         {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirements"},
         {},
         {dev::default_types::integer, dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::integer, dev::default_types::TODO},
         },
        { //ammo
         {"base_dmg", "ammo_type"},
         {},
         {dev::default_types::integer, dev::default_types::ammo_type},
         },
        { //armor
         {"armor_slot", "armor_points", "armor_bonus"},
         {},
         {dev::default_types::TODO, dev::default_types::u_short, dev::default_types::u_short},
         },
        { //consumable
         {"on_use", "uses_left", "use_energy_cost"},
         {},
         {dev::default_types::TODO, dev::default_types::short_t, dev::default_types::TODO},
         },
    }
    }
};

dev::info_field::info_field(QLabel* label, QWidget* field, QPoint location) : label_(label), field_(field) {
    label_->setGeometry(location.x(), location.y(), label_w, any_line_hight);
    field_->setGeometry(location.x()+ label_w + gap, location.y(), field_w , any_line_hight);
    label_->show();
    field_->show();
};
dev::info_field::~info_field() {
    if (label_->parent() == nullptr) {
        delete label_;
    }
    if (field_->parent() == nullptr) {
        delete field_;
    }
}

QWidget* dev::create_appropriate_field(dev::default_types field_type, QWidget* parent) {
    if (dev::is_type_linear(field_type)) {
        QLineEdit* field = new QLineEdit(parent);
        return field;
    }
    else {
        QComboBox* field = new QComboBox(parent);
        if (field_type == dev::default_types::boolean){
            field->addItems({"false", "true"});
        }
        if (field_type == dev::default_types::item_subtypes){
            field->addItems({"none", "weapon", "ammo", "armor", "consumable"});
        }
        else if (field_type == dev::default_types::damage_type){
            field->addItems({"bullet", "energy", "melee", "unarmed", "explosive"});
        }
        else if (field_type == dev::default_types::ammo_type){
            field->addItems({"none", "pistol", "shotgun", "rifle", "energy", "energy_shotgun", "rocket"});
        }
        return field;
    }
};

dev::object_dialog_window::object_dialog_window(dev::datatype object_type) : QWidget() {
    this->setBaseSize(400,800);
    short amount_of_fields;
    try {
        amount_of_fields = templates[object_type].keys_.length();
    } catch (...) {
        qInfo() << "[WARN][dev::build_dialog] tried to build a window for non existing type of object";
        amount_of_fields = 0;
    }

    int ypos = 10;
    for (short i = 1; i < amount_of_fields; ++i) {
        QString current_key = templates[object_type].keys_[i];
        dev::default_types current_type = templates[object_type].types_[i];
        QLabel* l = new QLabel(this);
        l->setText(current_key);

        QWidget* f = dev::create_appropriate_field(current_type, this);

        dev::info_field whole_field = {l, f, QPoint(10, ypos)};
        labels.emplace_back(l);
        fields.emplace_back(f);

        ypos += any_line_hight + 5;
    }
    this->show();
    qInfo() << "const worked";
}


void dev::object_dialog_window::type_chosen() {
    qInfo() << '1';
}
