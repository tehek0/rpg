#pragma once
#include <QWidget>
#include <QLabel>


namespace dev {

class custom_window : public QWidget {
public:
    custom_window();
protected:
    int ypos = 10;
};

class help_window : public custom_window {
private:
    std::vector<QLabel*> labels_;
public:
    help_window();
};

void call_reset_id_button_dialog();
void call_choose_exe_path();
}