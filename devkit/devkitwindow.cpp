#include "devkitwindow.h"
#include "ui_devkitwindow.h"
//#include "item_dialog.h"
#include <QLabel>
#include <QLineEdit>
#include "header/object_dialogs.hpp"

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

}

void DevkitWindow::on_createEntity_pb_clicked()
{
    dev::object_dialog_window* entity_w = new dev::object_dialog_window(dev::datatype::entity);
}

