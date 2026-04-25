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


anim_sequence animated_displayable::get_anim_sequence() {
    return _anim_sequence;
}
transpos animated_displayable::get_transpos() {
    return _transpos;
}
anim animated_displayable::get_current_anim() {
    return _anim_sequence.anims[_anim_sequence.current_anim_id];
}

void animated_displayable::set_anim_sequence(anim_sequence& anim_sequence_) {
    _anim_sequence = anim_sequence_;
}

void animated_displayable::set_current_anim(anim& anim_) {
    _anim_sequence.ticks_passed = 0;
    _anim_sequence.current_frame = 0;
    _anim_sequence.anims[_anim_sequence.current_anim_id] = anim_;
}
void animated_displayable::set_current_frame(unsigned int current_frame) {
    _anim_sequence.current_frame = current_frame;
    _anim_sequence.ticks_passed = 0;
}
void animated_displayable::switch_paused() {
    _anim_sequence.paused = (_anim_sequence.paused ? false : true);
}
void animated_displayable::set_transpos(transpos &transpos_) {
    _transpos = transpos_;
}
void animated_displayable::set_swap_destinations(unsigned int amount) {
    _transpos.times_to_swap_destinations = amount;
}
void animated_displayable::add_swap_destinations() {
    ++_transpos.times_to_swap_destinations;
}
void animated_displayable::move_to(QPoint &coord) {
    QSize size = _disp->size();
    QRect new_pos = QRect(coord, size);
    _disp->setGeometry(new_pos);
}

float animated_displayable::smoothstep_algorythm(float steps, float required_steps) {
    float process = steps / required_steps;
    if (process > 1.0f) {
        process = 1.0f;
    }
    else if (process < 0.0f) {
        process = 0.0f;
    }
    return process * process * (3.0f - 2.0f * process);
}

float animated_displayable::linear_algorythm(float steps, float required_steps) {
    float process = steps / required_steps;
    return process;
}

float animated_displayable::bounce_in_algorythm(float steps, float required_steps) {
    float process = steps / required_steps;
    process = 1.0f - process;
    if (process < 1.0f / 2.75f) {
        return 1.0f - (7.5625f * process * process);
    } else if (process < 2.0f / 2.75f) {
        process -= 1.5f / 2.75f;
        return 1.0f - (7.5625f * process * process + 0.75f);
    } else if (process < 2.5f / 2.75f) {
        process -= 2.25f / 2.75f;
        return 1.0f - (7.5625f * process * process + 0.9375f);
    } else {
        process -= 2.625f / 2.75f;
        return 1.0f - (7.5625f * process * process + 0.984375f);
    }
}

float animated_displayable::bounce_out_algorythm(float steps, float required_steps) {
    float process = steps / required_steps;
    if (process < (1.0f / 2.75f)) {
        return 7.5625f * process * process;
    } else if (process < (2.0f / 2.75f)) {
        return 7.5625f * (process -= (1.5f / 2.75f)) * process + 0.75f;
    } else if (process < (2.5f / 2.75f)) {
        return 7.5625f * (process -= (2.25f / 2.75f)) * process + 0.9375f;
    }
    else {
        return 7.5625f * (process -= (2.625f / 2.75f)) * process + 0.984375f;
    }
}

float animated_displayable::instant_algorythm(float steps, float required_steps) {
    if (steps < required_steps)
        return 0.0f;

    return 1.0f;
}

void animated_displayable::begin_step(QPoint& destination, unsigned int steps, transpos_algs alg) {
    _transpos.step = 0;
    _transpos.required_steps = steps;
    _transpos.start_destination = _disp->pos();
    _transpos.final_destination = destination;
    _transpos.algorithm = alg;
    if (_transpos.has_reached_destination == true) {
        connect(global::timer, &QTimer::timeout, this, &animated_displayable::next_step);
    }
    _transpos.has_reached_destination = false;

}
