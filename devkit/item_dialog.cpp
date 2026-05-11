#include "item_dialog.h"
#include "ui_item_dialog.h"
#include "header/id_support.hpp"
#include <fstream>

enum class item_type {
    none,
    weapon,
    ammo,
    armor,
    consumable
} type;
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

item_dialog::item_dialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::item_dialog)
{
    ui->setupUi(this);

    on_type_none_clicked();
    ui->sellable_yes->click();
}

item_dialog::~item_dialog()
{
    delete ui;
}

void item_dialog::on_save_clicked()
{
    unsigned long long id = dev::throw_id(dev::datatype::enemy);
    QString name = ui->name->text();
    QString desc = ui->desc->text();
    QString asset = ui->asset->text();
    unsigned int max_stack_size = ui->max_stack_size->text().toUInt();
    float base_weight = ui->base_weight->text().toFloat();
    unsigned int base_cost = ui->base_cost->text().toUInt();
    bool sellable;
    if (ui->sellable_yes->isChecked()) {
        sellable = true;
    }
    else {
        sellable = false;
    }
    js item;
    item["name"] = name.toStdString();
    item["desc"] = desc.toStdString();
    item["asset"] = asset.toStdString();
    item["stack"] = max_stack_size;
    item["base_weight"] = base_weight;
    item["base_cost"] = base_cost;
    item["sellable"] = sellable;
    if (type == item_type::weapon) {
        item["base_dmg"] = ui->add_info1->text().toInt();
        item["damage_type"] = ui->add_info2->text().toStdString();
        item["ammo_type"] = ui->add_info3->text().toStdString();
        item["energy_cost"] = ui->add_info4->text().toShort();
        item["requirements"] = ui->add_info5->text().toStdString();
    }

    QString filename = QString("objects/items/item_%1").arg(id);
    std::ofstream file(filename.toStdString());
}


void item_dialog::on_reset_clicked()
{
    ui->add_info1->clear();
    ui->add_info2->clear();
    ui->add_info3->clear();
    ui->add_info4->clear();
    ui->add_info5->clear();
    ui->name->clear();
    ui->desc->clear();
    ui->asset->clear();
    ui->max_stack_size->clear();
    ui->base_weight->clear();
    ui->base_cost->clear();
    ui->sellable_yes->toggle();
    on_type_none_clicked();
}


void item_dialog::on_type_none_clicked()
{
    ui->wl2->hide();
    ui->wl4->hide();
    ui->awl3->hide();
    ui->awl1->hide();
    ui->arl1->hide();
    ui->arl2->hide();
    ui->arl4->hide();
    ui->arwl5->hide();
    ui->cl1->hide();
    ui->cl2->hide();
    ui->add_info1->hide();
    ui->add_info2->hide();
    ui->add_info3->hide();
    ui->add_info4->hide();
    ui->add_info5->hide();
    type = item_type::none;
}


void item_dialog::on_type_weapon_clicked()
{
    ui->wl2->show();
    ui->wl4->show();
    ui->awl3->show();
    ui->awl1->show();
    ui->arl1->hide();
    ui->arl2->hide();
    ui->arl4->hide();
    ui->arwl5->show();
    ui->cl1->hide();
    ui->cl2->hide();
    ui->add_info1->show();
    ui->add_info2->show();
    ui->add_info3->show();
    ui->add_info4->show();
    ui->add_info5->show();
    type = item_type::weapon;
}


void item_dialog::on_type_ammo_clicked()
{
    ui->wl2->hide();
    ui->wl4->hide();
    ui->awl3->show();
    ui->awl1->show();
    ui->arl1->hide();
    ui->arl2->hide();
    ui->arl4->hide();
    ui->arwl5->hide();
    ui->cl1->hide();
    ui->cl2->hide();
    ui->add_info1->show();
    ui->add_info2->hide();
    ui->add_info3->show();
    ui->add_info4->hide();
    ui->add_info5->hide();
    type = item_type::ammo;
}


void item_dialog::on_type_armor_clicked()
{
    ui->wl2->hide();
    ui->wl4->hide();
    ui->awl3->hide();
    ui->awl1->hide();
    ui->arl1->show();
    ui->arl2->show();
    ui->arl4->show();
    ui->arwl5->show();
    ui->cl1->hide();
    ui->cl2->hide();
    ui->add_info1->show();
    ui->add_info2->show();
    ui->add_info3->hide();
    ui->add_info4->show();
    ui->add_info5->show();
    type = item_type::armor;
}


void item_dialog::on_type_consumable_clicked()
{
    ui->wl2->hide();
    ui->wl4->hide();
    ui->awl3->hide();
    ui->awl1->hide();
    ui->arl1->hide();
    ui->arl2->hide();
    ui->arl4->hide();
    ui->arwl5->hide();
    ui->cl1->show();
    ui->cl2->show();
    ui->add_info1->show();
    ui->add_info2->show();
    ui->add_info3->hide();
    ui->add_info4->hide();
    ui->add_info5->hide();
    type = item_type::consumable;
}

