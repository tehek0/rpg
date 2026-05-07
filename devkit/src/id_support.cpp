#include "../header/id_support.hpp"
#include "../header/general.hpp"
#include "fstream"
#include <QDebug>
#include <QFileDialog>

//Записывает айди id в файл со всеми айди для типа данных type
void dev::add_id_to_total(dev::datatype type, std::string id) {
    std::ifstream file(paths_to_ids[type]);
    std::string data;

    if (!file.is_open()) {
        qInfo() << "[add_id_to_total] file isn't open";
    }
    else {
        std::getline(file, data);
        data = data + txt_separator + id;

        file.close();

        std::ofstream file(paths_to_ids[type]);
        file << data;
        file.close();
    }
}
//Удаляет айди id из файла со всеми айди для типа данных type
void dev::remove_id_from_total(dev::datatype type, std::string id) {
    std::ifstream file(paths_to_ids[type]);
    std::string data;

    if (!file.is_open()) {
        qInfo() << "[remove_id_from_total] file isn't open";
    }
    else {
        std::getline(file, data);
        QString t = QString::fromStdString(data);
        QStringList ids = t.split(txt_separator,Qt::SkipEmptyParts);
        for(QString t_id : ids) {
            if (t_id == id) {
                ids.removeAt(ids.indexOf(t_id));
                break;
            }
        }

        file.close();

        std::ofstream file(paths_to_ids[type]);
        QString t_new;
        for(QString t_id : ids) {
            t_new = t_new + txt_separator + t_id;
        }

        file << t_new.toStdString();
        file.close();
    }
}

//Выдаёт незанятый для данного типа данных идентеикатор
//Первый байт всегда зависит от типа данных объекта
// const std::string dev::generate_id(dev::datatype type) {
//     std::string str_type = datatypes_to_string[type];
//     std::fstream file(paths_to_ids[type]);
//     short type_byte = type;

//     js all_data = js::parse(file);
//     js data = all_data[type];


//     QString id = QString("%1%2").arg(type_byte).arg( + 1000);

//     add_to_total(type);
//     file << data.dump(js_indent);

//     qInfo() << id;
//     file.close();
//     return id;
// }

