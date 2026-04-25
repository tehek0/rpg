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

struct anim {
    QString name;
    unsigned int ticks_to_move;
    unsigned int last_frame;
    bool is_looping = true;
    bool restart_after_pause = false;
    anim(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_): name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_)
    {}
    anim(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_, bool is_looping_, bool restart_after_pause_): name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_), is_looping(is_looping_), restart_after_pause(restart_after_pause_)
    {}
};

struct anim_sequence {
    std::vector<anim> anims;
    unsigned int ticks_passed = 0;
    unsigned int current_anim_id = 0;
    unsigned int current_frame = 0;
    bool paused = false;
    anim_sequence() = default;
    template<typename... Args>
    anim_sequence(unsigned int current_anim_id_, Args... args)
    {
        current_anim_id = current_anim_id_;
        static_assert((std::is_constructible_v<anim, Args> && ...));
        (anims.push_back(std::forward<Args>(args)), ...);
    }
};

enum class transpos_algs {
    smoothstep,
    linear,
    bounce_in,
    bounce_out,
    instant
};

struct transpos {
    transpos_algs algorithm = transpos_algs::linear;
    QPoint final_destination;
    QPoint start_destination;
    unsigned int step = 0;
    unsigned int required_steps;
    unsigned int times_to_swap_destinations = 0;
    bool has_reached_destination = true;
    void finish();
    transpos() = default;
    transpos(QPoint& start_destination_, QPoint& final_destination_, unsigned int required_steps_): start_destination(start_destination_), final_destination(final_destination_), required_steps(required_steps_)
    {}
    transpos(QPoint& start_destination_, QPoint& final_destination_, unsigned int required_steps_, transpos_algs algorithm_): start_destination(start_destination_), final_destination(final_destination_), required_steps(required_steps_), algorithm(algorithm_)
    {}
};

class animated_displayable: public displayable {

    Q_OBJECT


private:
    static float smoothstep_algorythm(float steps, float required_steps);
    static float linear_algorythm(float steps, float required_steps);
    static float bounce_in_algorythm(float steps, float required_steps);
    static float bounce_out_algorythm(float steps, float required_steps);
    static float instant_algorythm(float steps, float required_steps);
protected:
    anim_sequence _anim_sequence;
    transpos _transpos;
public slots:
    void next_frame() {
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

    void next_step() {
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
public:
    animated_displayable() = default;
    animated_displayable(MainWindow* w, bool clickable, QPoint& coord, QSize& size, QString& sprite_family, QString& name, anim_sequence anim_sequence_): displayable(w, clickable, coord, size, sprite_family, name) {
        _anim_sequence = anim_sequence_;
        _disp->setStyleSheet(QString("border-image: url(:animated/%1/base_sprite.png);").arg(_sprite_family));
        connect(global::timer, &QTimer::timeout, this, &animated_displayable::next_frame);
    }
    anim_sequence get_anim_sequence();
    anim get_current_anim();
    transpos get_transpos();
    void set_anim_sequence(anim_sequence& anim_sequence_);
    void set_current_anim(anim& anim_);
    void set_current_frame(unsigned int current_frame);
    void switch_paused();
    void set_transpos(transpos& transpos_);
    void set_swap_destinations(unsigned int amount);
    void add_swap_destinations();

    void move_to(QPoint& coord);
    void begin_step(QPoint& destination, unsigned int steps, transpos_algs alg);
};
