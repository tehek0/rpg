#pragma once
#include <QPushButton>
// TODO: класс для чтения и хранения всех данных
class map_widget;

void saves_init(int amount_of_saves = 3);
void save_map(map_widget* map, int slot);
bool load_map(map_widget* map, int slot);
class slot: public QPushButton {

};
