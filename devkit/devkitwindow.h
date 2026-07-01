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

    void on_createArmor_bonus_pb_clicked();

    void on_createOn_use_pb_clicked();

    void on_delete_object_clicked();

    void on_createItemRequirements_pb_clicked();

    void on_createRequirements_pb_2_clicked();

    void on_help_clicked();

    void on_map_construct_clicked();

    void on_rebootIds_clicked();

private:
    std::vector<dev::custom_window*> windows;
        Ui::DevkitWindow *ui;
};
#endif // DEVKITWINDOW_H
