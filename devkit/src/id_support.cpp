#include "../header/id_support.hpp"
#include "../header/data/general.hpp"
#include "fstream"
#include <QDebug>
#include <QFileDialog>

std::string path("../../../");

void dev::set_ids_default_state() {
    std::ofstream default_ids(path + "ids.json");
    js default_object = js::object();
    short index = 0;
    for (const auto& str : datatypes_to_string) {
        if (index >= dev::datatype::location){
            QString q_last = QString(q_last_ptrn).arg(str);
            std::string last = q_last.toStdString();
            default_object[last] = 0;

            QString q_dangling = QString(q_dangling_ptrn).arg(str);
            std::string dangling = q_dangling.toStdString();
            default_object[dangling] = js::array();
        }

        ++index;
    }
    default_ids << default_object.dump(js_indent);
}

unsigned long long dev::throw_id(datatype type) {
    std::ifstream in_ids(path + "ids.json");
    if (!in_ids.is_open()) {
        in_ids.close();
        set_ids_default_state();
        in_ids.open(path + "ids.json");
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

    std::ofstream out_ids(path + "ids.json");
    out_ids.clear();
    out_ids << id_info.dump(js_indent);

    return throw_id;
}

void dev::remove_id(datatype type, unsigned long long id) {
    std::ifstream in_ids(path + "ids.json");
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

    std::ofstream out_ids(path + "ids.json");
    out_ids.clear();
    out_ids << id_info.dump(js_indent);
}

