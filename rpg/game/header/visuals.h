#pragma once
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <QTextBrowser>
#include "global.h"
#include "data/tooltip_types.h"

//визуальные компоненты игровых объектов

class tracked_button : public QPushButton {

    Q_OBJECT

signals:
    void request_tooltip();

protected:
    void enterEvent(QEnterEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
public:
    QTextBrowser* linked_tooltip = nullptr;
    tooltip_types tooltip = tooltip_types::disabled;
    tracked_button(): QPushButton()
    {
        this->setMouseTracking(true);
    }
};

class displayable: public QObject {

    Q_OBJECT

protected:
    QString _sprite_family;

public:
    tracked_button* _disp;

    displayable() = default;
    displayable(MainWindow* w, bool clickable, QPoint& coord, QSize& size, QString& sprite_family) : displayable() {
        _sprite_family = sprite_family;
        _disp = new tracked_button();
        _disp->setStyleSheet(QString("border-image: url(:/%1.png);").arg(_sprite_family));
        _disp->setGeometry(coord.x(),coord.y(), size.width(), size.height());
        _disp->setParent(w);
        if (!clickable) {
            _disp->setDisabled(true);
        }
    }

    virtual ~displayable() {
        delete _disp;
    }

    QString get_sprite_family();
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
    void next_frame();
    void next_step();
public:
    animated_displayable() = default;
    animated_displayable(MainWindow* w, bool clickable, QPoint& coord, QSize& size, QString& sprite_family, anim_sequence anim_sequence_): displayable(w, clickable, coord, size, sprite_family) {
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
