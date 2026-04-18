#include "../header/visuals.h"
#include <QLabel>
#include <QPushButton>

// displayable* displayable::create(MainWindow* w, bool clickable, QRect& coord_and_size, QString& sprite_family, QString& name){
//     displayable* disp = new displayable();
//     disp->setParent(w);
//     disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(sprite_family));
//     disp->setGeometry(coord_and_size);
//     disp->setObjectName(name);
//     if (!clickable) {
//         disp->setDisabled(true);
//     }
//     disp->show();
//     return disp;
//     delete disp;
// };

// clickable::clickable(MainWindow* w, QRect coord_and_size, QString sprite_family, QString name) {
//     QPushButton* clk = new QPushButton(w);
//     clk->setStyleSheet(QString("background-image: url(:/%1.png);").arg(sprite_family));
//     clk->setGeometry(coord_and_size);
//     clk->setObjectName(name);
//     clk->show();
// }

QString displayable::get_name() {
    return _name;
}
QString displayable::get_sprite_family() {
    return _sprite_family;
}
void displayable::set_name(QString name) {
    _name = name;
}
void displayable::set_sprite_family(QString sprite_family) {
    _sprite_family = sprite_family;
}


std::vector<animation> animated_displayable::get_animations() {
    return _animations;
}
unsigned int animated_displayable::get_ticks_passed() {
    return _ticks_passed;
}
unsigned int animated_displayable::get_current_animation_id() {
    return _current_animation_id;
}
animation animated_displayable::get_current_animation() {
    return _animations[_current_animation_id];
}
unsigned int animated_displayable::get_current_frame() {
    return _current_frame;
}
bool animated_displayable::get_paused() {
    return _paused;
}
void animated_displayable::set_animations(std::vector<animation>& animations) {
    _animations = animations;
}
void animated_displayable::set_ticks_passed(unsigned int ticks_passed) {
    _ticks_passed = ticks_passed;
}
void animated_displayable::set_current_animation_id(unsigned int current_animation_id) {
    _current_animation_id = current_animation_id;
}
void animated_displayable::set_current_animation(animation& anim) {
    _ticks_passed = 0;
    _current_frame = 0;
    _animations[_current_animation_id] = anim;
}
void animated_displayable::set_current_frame(unsigned int current_frame) {
    _current_frame = current_frame;
    _ticks_passed = 0;
}
void animated_displayable::set_paused(bool paused) {
    _paused = paused;
}
void animated_displayable::switch_paused() {
    _paused = (_paused ? false : true);
}
