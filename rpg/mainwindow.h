#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>


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

    Ui::MainWindow *ui;
public slots:
    void on_pushButton_clicked();
    void OnEntClicked();
private slots:
    void on_inventory_b_clicked();
    void on_map_b_clicked();

private:

};
#endif // MAINWINDOW_H
