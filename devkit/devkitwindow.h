#ifndef DEVKITWINDOW_H
#define DEVKITWINDOW_H

#include <QMainWindow>

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

private:
    Ui::DevkitWindow *ui;
};
#endif // DEVKITWINDOW_H
