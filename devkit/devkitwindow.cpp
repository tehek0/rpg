#include "devkitwindow.h"
#include "ui_devkitwindow.h"
//#include "item_dialog.h"
#include <QLabel>
#include <QLineEdit>


DevkitWindow::DevkitWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DevkitWindow)
{
    ui->setupUi(this);
}

DevkitWindow::~DevkitWindow()
{
    delete ui;
}

void DevkitWindow::on_delete_object_clicked()
{
    dev::object_dialog_window* delete_w = new dev::object_dialog_window(dev::datatype::erased);
    windows.emplace_back(delete_w);
}

void DevkitWindow::on_createItem_pb_clicked()
{
    dev::object_dialog_window* item_w = new dev::object_dialog_window(dev::datatype::item);
    windows.emplace_back(item_w);

}

void DevkitWindow::on_createEntity_pb_clicked()
{
    dev::object_dialog_window* entity_w = new dev::object_dialog_window(dev::datatype::entity);
    windows.emplace_back(entity_w);
}


void DevkitWindow::on_createRequirements_pb_clicked()
{
    dev::object_dialog_window* requirement_w = new dev::object_dialog_window(dev::datatype::requirement);
    windows.emplace_back(requirement_w);
}


void DevkitWindow::on_createArmor_bonus_pb_clicked()
{
    dev::object_dialog_window* armot_bonus_w = new dev::object_dialog_window(dev::datatype::armor_bonus);
    windows.emplace_back(armot_bonus_w);
}


void DevkitWindow::on_createOn_use_pb_clicked()
{
    dev::object_dialog_window* one_use_w = new dev::object_dialog_window(dev::datatype::on_use);
    windows.emplace_back(one_use_w);
}

