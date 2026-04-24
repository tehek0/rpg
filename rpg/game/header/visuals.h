#pragma once
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include "global.h"

//визуальные компоненты игровых объектов

class displayable: public QObject {

    Q_OBJECT

protected:
    QString _name;
    QString _sprite_family;

public:
    QPushButton* _disp;

    displayable() = default;
    displayable(MainWindow* w, bool clickable, QPoint& coord, QSize& size, QString& sprite_family, QString& name) : displayable() {
        _name = name;
        _sprite_family = sprite_family;
        _disp = new QPushButton();
        _disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(_sprite_family));
        _disp->setGeometry(coord.x(),coord.y(), size.width(), size.height());
        _disp->setObjectName(name);
        _disp->setParent(w);
        if (!clickable) {
            _disp->setDisabled(true);
        }
    }

    virtual ~displayable() {
        delete _disp;
    }

    QString get_name();
    QString get_sprite_family();
    void set_name(QString name);
    void set_sprite_family(QString sprite_family);
};

struct animation {
    QString name;
    unsigned int ticks_to_move;
    unsigned int last_frame;
    bool is_looping = true;
    bool restart_after_pause = false;
    animation(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_): name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_)
    {}
    animation(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_, bool is_looping_, bool restart_after_pause_): name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_), is_looping(is_looping_), restart_after_pause(restart_after_pause_)
    {}
};

struct transposition {
    QPoint final_destination;
    QPoint start_destination;
    unsigned int step;
    unsigned int required_steps;
    bool has_reached_destination;
};

struct animation_sequencer {
    std::vector<animation> animations;
    unsigned int ticks_passed;
    unsigned int current_animation_id;
    unsigned int current_frame;
    bool paused;
    animation_sequencer(): ticks_passed(0), current_animation_id(0), current_frame(0), paused(false)
    {}
    template<typename... Args>
    animation_sequencer(Args&&... args): ticks_passed(0), current_animation_id(0), current_frame(0), paused(false)
    {
        static_assert((std::is_constructible_v<animation, Args&&> && ...));
        (animations.push_back(std::forward<Args>(args)), ...);
    }
    template<typename... Args>
    animation_sequencer(int current_animation_id_, Args&&... args): animation_sequencer(args), ticks_passed(0), current_animation_id(current_animation_id_), current_frame(0)
    {}
};

class animated_displayable: public displayable {

    Q_OBJECT


private:
    static float smoothstep_algorythm(float steps, float required_steps);
protected:
    // animation_sequencer _animation_sequencer;
    std::vector<animation> _animations;
    unsigned int _ticks_passed = 0;
    unsigned int _current_animation_id = 0;
    unsigned int _current_frame = 0;
    bool _paused = false;

    QPoint _final_destination;
    QPoint _start_destination;
    unsigned int _step;
    unsigned int _required_steps;
    bool _has_reached_destination = true;
public slots:
    void next_frame() {
        if (_disp->isHidden() || _paused || _animations.size() == 0) {
            if (_animations[_current_animation_id].restart_after_pause == true && _current_frame != 0) {
                _current_frame = 0;
                _ticks_passed = 0;
            }
            return;
        }
        ++_ticks_passed;
        if (_ticks_passed > _animations[_current_animation_id].ticks_to_move) {
            _ticks_passed = 0;
            ++_current_frame;
            if (_current_frame > _animations[_current_animation_id].last_frame) {
                if (_animations[_current_animation_id].is_looping == false) {
                    this->set_paused(true);
                    _current_frame = 0;
                    return;
                }
                _current_frame = 0;
            }
            _disp->setStyleSheet(QString("border-image: url(:/animated/%1/%2/frame%3.png);").arg(_sprite_family).arg(_animations[_current_animation_id].name).arg(_current_frame));
        }
    }

    void smooth_step() {
        ++_step;
        if (_step > _required_steps) {
            disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::smooth_step);
            _has_reached_destination = true;
        }
        int set_x = _final_destination.x() - _start_destination.x();
        int set_y = _final_destination.y() - _start_destination.y();
        float coef = smoothstep_algorythm(_step, _required_steps);
        QPoint new_point = QPoint(_start_destination.x() + (set_x * coef), _start_destination.y() + (set_y * coef));
        this->move_to(new_point);
    }
public:
    animated_displayable() = default;
    template<typename... Args>
    animated_displayable(MainWindow* w, bool clickable, QPoint& coord, QSize& size, QString& sprite_family, QString& name, unsigned int start_from_animation, Args&&... args): displayable(w, clickable, coord, size, sprite_family, name) {
        static_assert((std::is_constructible_v<animation, Args&&> && ...));
        (_animations.push_back(std::forward<Args>(args)), ...);
        _current_animation_id = start_from_animation;
        _disp->setStyleSheet(QString("border-image: url(:animated/%1/base_sprite.png);").arg(_sprite_family));
        connect(global::timer, &QTimer::timeout, this, &animated_displayable::next_frame);
    }
    std::vector<animation> get_animations();
    unsigned int get_ticks_passed();
    unsigned int get_current_animation_id();
    animation get_current_animation();
    unsigned int get_current_frame();
    bool get_paused();
    void set_animations(std::vector<animation>& animations);
    void set_ticks_passed(unsigned int ticks_passed);
    void set_current_animation_id(unsigned int current_animation_id);
    void set_current_animation(animation& anim);
    void set_current_frame(unsigned int current_frame);
    void set_paused(bool paused);
    void switch_paused();

    void move_to(QPoint& coord);
    void begin_smooth_step(QPoint& destination, unsigned int steps);
};
