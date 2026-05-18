#include "item_dialog.h"
#include "ui_item_dialog.h"
#include "header/create_object.hpp"
#include "header/object_subclasses.hpp"


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

    QString name = ui->name->text();
    QString desc = ui->desc->text();
    QString asset = ui->asset->text();
    QString max_stack_size = ui->max_stack_size->text();
    QString base_weight = ui->base_weight->text();
    QString base_cost = ui->base_cost->text();
    QString sellable;
    if (ui->sellable_yes->isChecked()) {
        sellable = "1";
    }
    else {
        sellable = "0";
    }
    dev::object_data item = {
        {"name","desc","asset","max_stack_size","base_weight","base_cost","sellablle"},
        {name, desc, asset, max_stack_size, base_weight, base_cost, sellable},
        {dev::default_types::qstring, dev::default_types::qstring, dev::default_types::qstring, dev::default_types::u_integer, dev::default_types::u_integer,
         dev::default_types::u_integer, dev::default_types::boolean},
    };
    if (dev::subcl::item_subtypes == dev::subcl::item_subtypes::weapon) {
        for(QString key : {"base_dmg", "damage_type", "ammo_type", "energy_cost", "requirements"}) {
            item.keys_.append(key);
        }
        for(QString value : {ui->add_info1->text(), ui->add_info2->text(), ui->add_info3->text(), ui->add_info4->text(), ui->add_info5->text()}) {
            item.values_.append(value);
        }
        for(dev::default_types type : {dev::default_types::integer, dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::integer,
                                        dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::short_t}) {
            item.types_.emplace_back(type);
        }
    }
    else if (dev::subcl::item_subtypes == dev::subcl::item_subtypes::ammo) {
        for(QString key : {"base_dmg", "ammo_type"}) {
            item.keys_.append(key);
        }
        for(QString value : {ui->add_info1->text(), ui->add_info2->text(), ui->add_info3->text(), ui->add_info4->text(), ui->add_info5->text()}) {
            item.values_.append(value);
        }
        for(dev::default_types type : {dev::default_types::integer, dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::integer,
                                        dev::default_types::damage_type, dev::default_types::ammo_type, dev::default_types::short_t}) {
            item.types_.emplace_back(type);
        }
    }

    dev::create_object(dev::datatype::item, item);
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
    dev::subcl::item_subtypes = dev::subcl::item_subtypes::none;
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
    dev::subcl::item_subtypes = dev::subcl::item_subtypes::weapon;
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
    dev::subcl::item_subtypes = dev::subcl::item_subtypes::ammo;
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
    dev::subcl::item_subtypes = dev::subcl::item_subtypes::armor;
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
    dev::subcl::item_subtypes = dev::subcl::item_subtypes::consumable;
}

