#include "devkitwindow.h"
#include "ui_devkitwindow.h"
#include "header/id_support.hpp"
#include "item_dialog.h"

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

void DevkitWindow::on_pushButton_2_clicked()
{
    //dev::add_id_to_total(dev::datatype::quest, "341124");
    dev::remove_id_from_total(dev::datatype::quest, "2121");
}


void DevkitWindow::on_createItem_pb_clicked()
{
    item_dialog* item = new item_dialog();
    item->show();

}
