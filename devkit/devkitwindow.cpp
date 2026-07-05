#include "devkitwindow.h"
#include "ui_devkitwindow.h"
#include <QLabel>
#include <QLineEdit>
#include "header/data/general.hpp"
#include "header/map_construct.hpp"

DevkitWindow::DevkitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevkitWindow)
{
    ui->setupUi(this);
    dev::ui::inform = ui->inform_l;
    dev::call_choose_exe_path();

}

DevkitWindow::~DevkitWindow()
{
    delete ui;
}

void DevkitWindow::on_delete_object_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::erased);
    windows.emplace_back(w);
}

void DevkitWindow::on_createItem_pb_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::item);
    windows.emplace_back(w);

}

void DevkitWindow::on_createArmor_bonus_pb_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::armor_bonus);
    windows.emplace_back(w);
}


void DevkitWindow::on_createOn_use_pb_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::on_use);
    windows.emplace_back(w);
}

void DevkitWindow::on_createItemRequirements_pb_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::item_requirement);
    windows.emplace_back(w);
}


void DevkitWindow::on_createRequirements_pb_2_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::requirement);
    windows.emplace_back(w);
    w->change_subfields(0);
}


void DevkitWindow::on_help_clicked()
{
    dev::help_window* w = new dev::help_window();
    windows.emplace_back(w);
}


void DevkitWindow::on_map_construct_clicked()
{
    dev::map_construct_window* w = new dev::map_construct_window();
    windows.emplace_back(w);
}

void DevkitWindow::on_rebootIds_clicked()
{
    dev::call_reset_id_button_dialog();
}


void DevkitWindow::on_createTrade_pb_clicked()
{
    dev::object_dialog_window* w = new dev::object_dialog_window(dev::datatype::trade);
    windows.emplace_back(w);
    w->change_subfields(0);
}

