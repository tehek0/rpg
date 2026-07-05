#include "../header/config.h"
#include "../header/global.h"
#include <QString>
#include <QRegularExpression>
#include <string>

#define DEFAULT_MASTER_VOLUME 1
#define DEFAULT_SFX_VOLUME 1
#define DEFAULT_MUSIC_VOLUME 0.5

void change_cfg(float master_volume, float sfx_volume, float music_volume) {
    std::ofstream cfg("settings.cfg");
    cfg.clear();
    cfg << QString("master_volume=%1\nsfx_volume=%2\nmusic_volume=%3").arg(master_volume).arg(sfx_volume).arg(music_volume).toStdString();
}

void default_cfg() {
    change_cfg(DEFAULT_MASTER_VOLUME, DEFAULT_SFX_VOLUME, DEFAULT_MUSIC_VOLUME);
}

void load_cfg() {
    std::ifstream cfg("settings.cfg");
    if (cfg.is_open() != true) {
        default_cfg();
        cfg.open("settings.cfg");
    }
    apply_cfg(cfg);
}

void abort_cfg() {
    qInfo() << "cfg aborted";
    default_cfg();
    load_cfg();
}

void apply_cfg(std::ifstream& config) {
    if (config.peek() == config.eof()) {
        abort_cfg();
        return;
    }
    std::string line;
    char argument_id = 0;
    while (std::getline(config, line)) {
        switch(argument_id) {
            case 0: {
                QString q_line = QString::fromStdString(line);
                QRegularExpression r_e("^master_volume=[10]\\.?\\d*$");
                if (r_e.match(q_line).hasMatch() == true) {
                    qInfo() << q_line;
                    q_line.replace("master_volume=","");
                    global::master_volume = q_line.toDouble();
                    break;
                } else {
                    abort_cfg();
                    return;
                }
            }
            case 1: {
                QString q_line = QString::fromStdString(line);
                QRegularExpression r_e("^sfx_volume=[10]\\.?\\d*$");
                if (r_e.match(q_line).hasMatch() == true) {
                    qInfo() << q_line;
                    q_line.replace("sfx_volume=","");
                    global::sfx_volume = q_line.toDouble();
                    break;
                } else {
                    abort_cfg();
                    return;
                }
            }
            case 2: {
                QString q_line = QString::fromStdString(line);
                QRegularExpression r_e("^music_volume=[10]\\.?\\d*$");
                if (r_e.match(q_line).hasMatch() == true) {
                    qInfo() << q_line;
                    q_line.replace("music_volume=","");
                    global::music_volume = q_line.toDouble();
                    break;
                } else {
                    abort_cfg();
                    return;
                }
            }
            default: {
                abort_cfg();
                return;
            }
        }
        ++argument_id;
    }
    if (argument_id != 3) {
        abort_cfg();
    }
}
