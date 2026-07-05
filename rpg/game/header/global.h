#pragma once
#include <QTimer>
#include <QApplication>
#include <filesystem>
#include "music.h"
#include "mainwindow.h"
// Тест глобального пространства имён
class player;
namespace global {
    inline player* player_;
    inline unsigned int selected_save;
    inline std::filesystem::path root_path;
    inline QTimer* timer = new QTimer;
    inline const short tick_timeout = 5;
    inline int argc;
    inline char** argv;
    inline double master_volume = 1.0f;
    inline double music_volume = 0.5f;
    inline double sfx_volume = 0.75f;
    inline unsigned short window_width = 1920;
    inline unsigned short window_height = 1080;
    inline QApplication a(argc, argv);
    inline MainWindow w;
    inline music_manager* music = new music_manager();
}
