#include "../header/custom_window.h"
#include "../header/data/general.hpp"
#include "header/id_support.hpp"
#include "qdialog.h"
#include "qpushbutton.h"
#include "QFileDialog"

dev::custom_window::custom_window() : QWidget(){
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->show();
}

QStringList help_lines = {"Все созданные объекты будут сохранены в соседнюю для .exe файла папку objects. Компоненты будут лежать на уровень ниже в папке components.",
                          "1. [Только что созданного компонента нет в таблице]\n"
                          "Если интересущая вас таблица появилась при выборе подтипа, переключитете его и верните обратно."
                          "Иначе закройте/откройте заново окно создания. Такие действия обновят значения в таблице.",
                          "2. [В окне создания предмета ожидался компонент, но для него нет ни названия, ни таблицы.]\n"
                          "Скорее всего у вас не создано ни одного требуемого компонента. Когда доступных объектов нет, поле ввода не создаётся.",
                          };
dev::help_window::help_window() {
    this->setWindowTitle("Help");
    for (QString line : help_lines) {
        QLabel* one = new QLabel(line, this);
        one->setWordWrap(true);
        one->setGeometry(gap, ypos, field_w, any_line_hight);
        one->adjustSize();
        one->show();
        labels_.emplace_back(one);
        ypos += one->height() + gap;
    }
    this->adjustSize();
    this->show();
}

void dev::call_reset_id_button_dialog() {
    QDialog* ask = new QDialog();
    ask->setWindowTitle("Вы уверены?");
    ask->resize(340,200);
    QLabel* label = new QLabel("[Использовать только при расширении функционала\nдевкита для добавления нового типа данных.]\n"
                               "Это действие очистит историю созданных id.\nВсе текущие объекты выпадут из системы.\n"
                               "Это не фатально, но такого допускать не стоит.\nЭто приведёт к некорректной работе удаления объектов\n"
                               "Если у вас есть объекты 1 и 2, придётся нажать сохранение трижды,\nчтобы создался объект.\n", ask);
    QPushButton* ok = new QPushButton("ОК", ask);
    QPushButton* cancel = new QPushButton("Отмена", ask);
    label->setGeometry(gap, gap, field_w, any_line_hight);
    label->adjustSize();
    ok->setGeometry(gap, 150, button_side, button_side);
    cancel->setGeometry(gap*2 + button_side, 150, button_side, button_side);
    label->setWordWrap(true);

    QAbstractButton::connect(ok, &QPushButton::clicked, dev::set_ids_default_state);
    QAbstractButton::connect(ok, &QPushButton::clicked, [ask]() {ask->close();});
    QAbstractButton::connect(cancel, &QPushButton::clicked, [ask]() {ask->close();});
    ask->show();

}

void dev::call_choose_exe_path() {
    QString path = QFileDialog::getExistingDirectory(nullptr, "Выберите папку с .exe файлом игры","", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    path_to_rpg_exe = path;
}
