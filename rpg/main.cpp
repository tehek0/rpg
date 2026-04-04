#include "mainwindow.h"
#include <QApplication>
#include "game/header/inventory.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // w.showFullScreen();
    inventory* player_inventory = new inventory;
    armor test1("Тест шлема", "Это тестовый шлем", "no_assets/0", 1, 1, 15, 50, false, {0, {}}, armor_slot::head, 10, {});
    item test2("Тест предмета", "Это тестовый предмет", "no_assets/1", 4, 66, 1, 50, true);
    item test3("Тест предмета 2", "Это тестовый предмет", "no_assets/2", 2, 25, 1, 555, true);
    item test4("Тест предмета 3", "Это тестовый предмет", "no_assets/3", 6, 12, 1, 5, true);
    item test5("Тест предмета 4", "Это тестовый предмет", "no_assets/4", 1, 30, 1, 55555, true);
    item test6("Тест предмета 2", "Это тестовый предмет", "no_assets/2", 8, 25, 1, 555, true);
    armor* test1_ptr = new armor;
    *test1_ptr = test1;
    item* test2_ptr = new item;
    *test2_ptr = test2;
    item* test3_ptr = new item;
    *test3_ptr = test3;
    item* test4_ptr = new item;
    *test4_ptr = test4;
    item* test5_ptr = new item;
    *test5_ptr = test5;
    item* test6_ptr = new item;
    *test6_ptr = test6;
    player_inventory->add_item(test1_ptr);
    player_inventory->add_item(test2_ptr);
    player_inventory->add_item(test3_ptr);
    player_inventory->add_item(test4_ptr);
    player_inventory->add_item(test5_ptr);
    player_inventory->add_item(test6_ptr);
    player_inventory->equip_armor_legs(test1_ptr);
    player_inventory->equip_armor_head(test1_ptr);
    player_inventory->equip_armor_legs(test2_ptr);
    for (size_t i = 0; i < player_inventory->get_items().size(); ++i) {
        qInfo() << player_inventory->get_items()[i]->get_name();
    }
    return a.exec();
}
