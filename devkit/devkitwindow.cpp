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

