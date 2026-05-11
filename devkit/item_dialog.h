#ifndef ITEM_DIALOG_H
#define ITEM_DIALOG_H

#include <QWidget>

namespace Ui {
class item_dialog;
}

class item_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit item_dialog(QWidget *parent = nullptr);
    ~item_dialog();

private slots:
    void on_save_clicked();

    void on_reset_clicked();

    void on_type_none_clicked();

    void on_type_weapon_clicked();

    void on_type_ammo_clicked();

    void on_type_armor_clicked();

    void on_type_consumable_clicked();

private:
    Ui::item_dialog *ui;
};

#endif // ITEM_DIALOG_H
