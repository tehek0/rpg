#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

enum class screens {
    map,
    inventory,
    stats,
    menu
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<QWidget*> on_screen;
    std::vector<QWidget*> on_inventory;
    std::vector<QWidget*> on_stats;
    std::vector<QWidget*> on_menu;
    std::vector<QWidget*> on_map;
    screens current_screen;

    Ui::MainWindow *ui;
public slots:
    void on_pushButton_clicked();
    void OnEntClicked();
    void on_menu_b_clicked();
private slots:
    void on_inventory_b_clicked();
    void on_map_b_clicked();
    void on_stats_b_clicked();
    void on_menu_toGame_clicked();

    void on_menu_exit_clicked();

private:

};
#endif // MAINWINDOW_H
