#ifndef DEVKITWINDOW_H
#define DEVKITWINDOW_H

#include <QMainWindow>
#include "header/object_dialogs.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class DevkitWindow;
}
QT_END_NAMESPACE

class DevkitWindow : public QMainWindow
{
    Q_OBJECT

public:
    DevkitWindow(QWidget *parent = nullptr);
    ~DevkitWindow();

private slots:


    void on_createItem_pb_clicked();

    void on_createEntity_pb_clicked();
    void on_createRequirements_pb_clicked();

private:
    Ui::DevkitWindow *ui;
    std::vector<dev::object_dialog_window*> windows;
};
#endif // DEVKITWINDOW_H
