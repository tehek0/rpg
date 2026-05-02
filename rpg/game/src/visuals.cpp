#include "../header/visuals.h"
#include <QLabel>
#include <QPushButton>
#include <QEnterEvent>
#include <QTextBrowser>

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


QString displayable::get_sprite_family() {
    return _sprite_family;
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

void animated_displayable::next_frame() {
    if (_disp->isHidden() || _anim_sequence.paused || _anim_sequence.anims.size() == 0) {
        if (_anim_sequence.anims[_anim_sequence.current_anim_id].restart_after_pause == true && _anim_sequence.current_frame != 0) {
            _anim_sequence.current_frame = 0;
            _anim_sequence.ticks_passed = 0;
        }
        return;
    }
    ++_anim_sequence.ticks_passed;
    if (_anim_sequence.ticks_passed > _anim_sequence.anims[_anim_sequence.current_anim_id].ticks_to_move) {
        _anim_sequence.ticks_passed = 0;
        ++_anim_sequence.current_frame;
        if (_anim_sequence.current_frame > _anim_sequence.anims[_anim_sequence.current_anim_id].last_frame) {
            if (_anim_sequence.anims[_anim_sequence.current_anim_id].is_looping == false) {
                _anim_sequence.paused = true;
                _anim_sequence.current_frame = 0;
                return;
            }
            _anim_sequence.current_frame = 0;
        }
        _disp->setStyleSheet(QString("border-image: url(:/animated/%1/%2/frame%3.png);").arg(_sprite_family).arg(_anim_sequence.anims[_anim_sequence.current_anim_id].name).arg(_anim_sequence.current_frame));
    }
}

void animated_displayable::next_step() {
    if (_disp->linked_tooltip != nullptr) {
        _disp->linked_tooltip->deleteLater();
        _disp->linked_tooltip = nullptr;
    }
    ++_transpos.step;
    if (_transpos.step > _transpos.required_steps) {
        if (_transpos.times_to_swap_destinations > 0) {
            --_transpos.times_to_swap_destinations;
            _transpos.step = 1;
            QPoint temp_destination = _transpos.final_destination;
            _transpos.final_destination = _transpos.start_destination;
            _transpos.start_destination = temp_destination;
        } else {
            disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::next_step);
            _transpos.has_reached_destination = true;
        }
    }
    int set_x = _transpos.final_destination.x() - _transpos.start_destination.x();
    int set_y = _transpos.final_destination.y() - _transpos.start_destination.y();
    float coef;
    switch(_transpos.algorithm)
    {
    case transpos_algs::smoothstep: {
        coef = smoothstep_algorythm(_transpos.step, _transpos.required_steps);
        break;
    }
    case transpos_algs::bounce_in: {
        coef = bounce_in_algorythm(_transpos.step,_transpos.required_steps);
        break;
    }
    case transpos_algs::bounce_out: {
        coef = bounce_out_algorythm(_transpos.step,_transpos.required_steps);
        break;
    }
    case transpos_algs::instant: {
        coef = instant_algorythm(_transpos.step,_transpos.required_steps);
        break;
    }
    default: {
        coef = linear_algorythm(_transpos.step,_transpos.required_steps);
    }
    }
    QPoint new_point = QPoint(_transpos.start_destination.x() + (set_x * coef), _transpos.start_destination.y() + (set_y * coef));
    this->move_to(new_point);
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

void tracked_button::enterEvent(QEnterEvent *event) {
    if (this->tooltip == tooltip_types::disabled)
        return;


    if (linked_tooltip != nullptr) {
        delete linked_tooltip;
        linked_tooltip = nullptr;
    }

    QTextBrowser* tooltip = new QTextBrowser(&global::w);
    tooltip->setGeometry(QRect(this->pos().x() + this->width() + 1, this->pos().y(), 150, 192));
    tooltip->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
    tooltip->show();
    linked_tooltip = tooltip;
    emit request_tooltip();
    int height = tooltip->document()->size().height();
    int frame_margin = tooltip->frameWidth() * 1.5f;
    int document_margin = tooltip->document()->documentMargin() * 1.5f;
    tooltip->setFixedHeight(height + frame_margin + document_margin);
    tooltip->document()->setTextWidth(tooltip->viewport()->width());
    if (tooltip->pos().x() + tooltip->size().width() >= 1920)
        tooltip->setGeometry(this->pos().x() - tooltip->size().width(), tooltip->pos().y(), tooltip->size().width(), tooltip->size().height());
}

void tracked_button::mouseMoveEvent(QMouseEvent *event) {
}

void tracked_button::leaveEvent(QEvent *event) {
    if (this->tooltip == tooltip_types::disabled)
        return;


    if (linked_tooltip != nullptr) {
        linked_tooltip->deleteLater();
        linked_tooltip = nullptr;
    }
}
