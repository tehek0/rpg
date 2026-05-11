#include "../header/id_support.hpp"
#include "../header/general.hpp"
#include "fstream"
#include <QDebug>
#include <QFileDialog>

<<<<<<< Updated upstream
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
=======
void dev::set_ids_default_state() {
    std::ofstream default_ids("ids.json");
    js default_object = js::object();
    for (const auto& str : datatypes_to_string) {
        QString q_last = QString(q_last_ptrn).arg(str);
        std::string last = q_last.toStdString();
        default_object[last] = 0;

        QString q_dangling = QString(q_dangling_ptrn).arg(str);
        std::string dangling = q_dangling.toStdString();
        default_object[dangling] = js::array();
    }
    default_ids << default_object.dump(js_indent);
}

unsigned long long dev::throw_id(datatype type) {
    std::ifstream in_ids("ids.json");
    if (!in_ids.is_open()) {
        in_ids.close();
        set_ids_default_state();
        in_ids.open("ids.json");
    }
    js id_info;
    try {
        id_info = js::parse(in_ids);
    } catch (...) {
        qInfo() << "[FATAL][dev::throw_id] ids.json is unparsable";
        exit(-1);
    }

    unsigned long long throw_id;
    QString q_dangling = QString(q_dangling_ptrn).arg(datatypes_to_string[type]);
    std::string dangling = q_dangling.toStdString();
    QString q_last = QString(q_last_ptrn).arg(datatypes_to_string[type]);
    std::string last = q_last.toStdString();

    if (!id_info[dangling].empty()) {
        throw_id = id_info[dangling][0];
        id_info[dangling].erase(id_info[dangling].begin());
    } else {
        throw_id = id_info[last].get<unsigned long long>() + 1;
        js temp_obj = throw_id;
        id_info[last].swap(temp_obj);
    }

    std::ofstream out_ids("ids.json");
    out_ids.clear();
    out_ids << id_info.dump(js_indent);

    return throw_id;
}

void dev::remove_id(datatype type, unsigned long long id) {
    std::ifstream in_ids("ids.json");
    js id_info;
    try {
        id_info = js::parse(in_ids);
    } catch (...) {
        qInfo() << "[FATAL][dev::remove_id] ids.json is unparsable";
        exit(-1);
    }
    QString q_last = QString(q_last_ptrn).arg(datatypes_to_string[type]);
    std::string last = q_last.toStdString();

    if (id_info[last].get<unsigned long long>() < id) {
        qInfo() << QString("[WARN][dev::remove_id] id %1 does not exist").arg(id);
        return;
    }

    QString q_dangling = QString(q_dangling_ptrn).arg(datatypes_to_string[type]);
    std::string dangling = q_dangling.toStdString();
    id_info[dangling].emplace_back(id);

    std::ofstream out_ids("ids.json");
    out_ids.clear();
    out_ids << id_info.dump(js_indent);
}
>>>>>>> Stashed changes

