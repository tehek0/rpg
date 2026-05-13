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

void DevkitWindow::on_createItem_pb_clicked()
{
    item_dialog* item = new item_dialog();
    item->show();

}
