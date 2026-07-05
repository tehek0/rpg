#pragma once
#include <QCursor>
#include <QString>
#include <mainwindow.h>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <vector>
#include <QTextBrowser>
#include <QPixmap>
#include <QPainter>
#include <QWheelEvent>
#include <QRegularExpression>
#include <QSoundEffect>
#include <QGraphicsLineItem>
#include <QLineEdit>
#include <QCheckBox>
#include <QFileInfo>
#include <QSlider>
#include <QDirListing>
#include <QList>
#include <algorithm>
#include "save.h"
#include "global.h"
#include "message.h"
#include "character.h"
#include "data/tooltip_types.h"
#include "data/inventory_contexts.h"
#include "data/biomes.h"
#include "enum_translation.h"

#include <qprogressbar.h>
#include <fstream>

class can_play_sound {
public:
    void play_sfx(QSoundEffect* sfx) {
        sfx->setVolume(global::master_volume * global::sfx_volume);
        sfx->play();
    }
};

//визуальные компоненты игровых объектов

class tracked_button : public QPushButton {

    Q_OBJECT

signals:
    void request_tooltip();
    void called_detor();

protected:
    void enterEvent(QEnterEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
public:
    QTextBrowser* linked_tooltip = nullptr;
    tooltip_types tooltip = tooltip_types::disabled;
    tracked_button(QWidget* parent = nullptr): QPushButton(parent)
    {
        this->setMouseTracking(true);
        this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    }

    ~tracked_button() {
        if (linked_tooltip != nullptr)
            linked_tooltip->deleteLater();
        emit called_detor();
    }
};

class unclickable_button: public tracked_button {

    Q_OBJECT

public:

    unclickable_button(QWidget* parent = nullptr): tracked_button(parent) {
        this->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
};

class displayable: public QObject {

    Q_OBJECT

protected:
    QString _sprite_family;

public slots:
    virtual void catch_disp_delete() {
        _disp = nullptr;
        this->deleteLater();
    }

public:
    tracked_button* _disp;

    displayable() = default;
    displayable(QString sprite_family, const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool clickable = true) : displayable() {
        _sprite_family = sprite_family;
        _disp = (clickable ? new tracked_button() : new unclickable_button());
        _disp->setStyleSheet(QString("border-image: url(assets:/pictures/%1.png);").arg(_sprite_family));
        _disp->setGeometry(coord.x(),coord.y(), size.width(), size.height());
        _disp->setParent(&global::w);
        _disp->setEnabled(clickable);
        connect(_disp, &tracked_button::called_detor, this, &displayable::catch_disp_delete);
    }

    virtual ~displayable() {
        if (_disp != nullptr)
            _disp->deleteLater();
    }

    QString get_sprite_family();
    void set_sprite_family(const QString& sprite_family);
};

struct anim {
    QString name;
    unsigned int ticks_to_move;
    unsigned int last_frame;
    bool is_looping = true;
    bool restart_after_pause = false;
    anim(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_): name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_)
    {}
    anim(QString name_, unsigned int ticks_to_move_, unsigned int last_frame_, bool is_looping_, bool restart_after_pause_):
        name(name_), ticks_to_move(ticks_to_move_), last_frame(last_frame_), is_looping(is_looping_), restart_after_pause(restart_after_pause_)
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
    transpos(QPoint& start_destination_, QPoint& final_destination_,
             unsigned int required_steps_):
        final_destination(final_destination_), start_destination(start_destination_),
        required_steps(required_steps_)
    {}
    transpos(QPoint& start_destination_, QPoint& final_destination_,
             unsigned int required_steps_, transpos_algs algorithm_):
        algorithm(algorithm_), final_destination(final_destination_),
        start_destination(start_destination_), required_steps(required_steps_)
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
    virtual void catch_disp_delete() {
        _disp = nullptr;
        this->disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::next_frame);
        this->disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::next_step);
        this->deleteLater();
    }
signals:
    void finished_animation();
    void reached_destination();
    void began_step();
public:
    animated_displayable() = default;
    animated_displayable(QString sprite_family, const anim_sequence& anim_sequence_ = anim_sequence(), const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool clickable = true): displayable(sprite_family, coord, size, clickable) {
        _anim_sequence = anim_sequence_;
        _disp->setStyleSheet(QString("border-image: url(assets:/pictures/animated/%1/base_sprite.png);").arg(_sprite_family));
        connect(global::timer, &QTimer::timeout, this, &animated_displayable::next_frame);
    }
    anim_sequence get_anim_sequence();
    anim get_current_anim();
    transpos get_transpos();
    void set_anim_sequence(anim_sequence& anim_sequence_);
    void set_current_anim(anim& anim_);
    void set_current_anim(unsigned int _current_anim_id);
    void set_current_frame(unsigned int current_frame);
    void switch_paused();
    void set_transpos(transpos& transpos_);
    void set_swap_destinations(unsigned int amount);
    void add_swap_destinations();

    virtual void move_to(QPoint& coord);
    void begin_step(const QPoint& destination, unsigned int steps, transpos_algs alg);
    void interrupt();
    void skip();

    virtual ~animated_displayable() {
        if (_disp != nullptr)
            _disp->deleteLater();
    }
};

class item_object: public displayable {

    Q_OBJECT

public slots:
    void markdown_item() {
        if (_disp->linked_tooltip == nullptr)
            return;
        if (linked_item == nullptr) {
            _disp->linked_tooltip->setText("Пустой слот экипировки");
            return;
        }
        _disp->linked_tooltip->setText(linked_item->get_tooltip_text() + (is_equipped ? QString("<p><center><font color=\"#0CCC3F\", font size=\"3\">(ЭКИПИРОВАНО)</font></center><\p>") : ""));
    }
    virtual void clicked() {
        emit click_send_to_parent(this);
    }
signals:
    void click_send_to_parent(item_object* obj); // нужно запрашивать контекст инвентаря у списка предметов, чтобы вызывать правильный попап
public:
    item* linked_item;
    unclickable_button* equipped_overlay;
    QLabel* stack_label;
    bool is_dummy;
    bool is_equipped = false;
    item_object() {
        is_dummy = true;
    } // это обязательно
    item_object(item* linked_item_, const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool clickable = true): displayable((linked_item_ == nullptr ? "null" : linked_item_->get_asset()), coord, size, clickable), linked_item(linked_item_)
    {
        is_dummy = false;
        equipped_overlay = new unclickable_button(_disp);
        equipped_overlay->resize(_disp->size());
        equipped_overlay->setStyleSheet("border-image: url(assets:/pictures/null.png);");
        stack_label = new QLabel(_disp);
        stack_label->setGeometry(_disp->geometry());
        stack_label->setAlignment(Qt::AlignRight | Qt::AlignBottom);
        stack_label->setStyleSheet(QString("color: rgb(255, 255, 255);\nfont: %1pt \"MS Gothic\";").arg(_disp->width() / 5));
        this->refresh_stack();
        _disp->tooltip = tooltip_types::item_display;
        connect(_disp, &tracked_button::request_tooltip, this, &item_object::markdown_item);
        connect(_disp, &tracked_button::clicked, this, &item_object::clicked);
    }
    void refresh_stack() {
        if (linked_item == nullptr) {
            stack_label->clear();
            return;
        }

        if (linked_item->get_stack() > 1) {
            stack_label->setText(QString("x%1").arg(linked_item->get_stack()));
            return;
        }

        stack_label->clear();
    }
    void set_equipped() {
        is_equipped = true;
        equipped_overlay->setStyleSheet("border-image: url(assets:/pictures/inventory_item_equipped_overlay.png);");
    }
    void set_deequipped() {
        is_equipped = false;
        equipped_overlay->setStyleSheet("border-image: url(assets:/pictures/null.png);");
    }
    void change_item(item* item_) {
        linked_item = item_;
        if (item_ == nullptr) {
            this->set_sprite_family("null");
            refresh_stack();
            return;
        }
        this->set_sprite_family(item_->get_asset());
        refresh_stack();
    }

};

class inventory_background : public QWidget {

    Q_OBJECT

signals:
    void scrolled(int delta);

public:
    QPixmap sprite;
    inventory_background(const QString& asset, QWidget* parent = nullptr): QWidget(parent) {
        sprite.load(QString("assets:/pictures/%1.png").arg(asset));
    }
    void paintEvent(QPaintEvent *event) {
        QPainter paint(this);
        if (sprite.isNull())
            return;

        paint.drawTiledPixmap(this->rect(), sprite);
    }
    void wheelEvent(QWheelEvent* event) {
        emit scrolled(event->angleDelta().y());
        event->accept();
    }
};

class inventory_object : public QWidget {

    Q_OBJECT
signals:
    void send_click(item_object* object);
    void send_equipped(item* item_);
    void send_deequipped(item* item_);
public slots:
    void process_item_click(item_object* item_obj) {
        // if (linked_inventory->is_equipped(item_obj->linked_item)) {
        //     linked_inventory->deequip(item_obj->linked_item);
        //     return;
        // }
        // linked_inventory->equip(item_obj->linked_item);

        emit send_click(item_obj);
    }
    void update(unsigned int slot, inv_update_context context_) {
        unsigned int lower_boundry = _scrolled_cols * _cols;
        unsigned int upper_boundry = lower_boundry + (_cols * _displayed_rows) - 1;
        if (context_ == inv_update_context::equipped) {
            emit send_equipped(linked_inventory->get_item(slot));
            if (slot < lower_boundry || slot > upper_boundry)
                return;
            item_objects[slot - lower_boundry]->set_equipped();
            return;
        }
        if (context_ == inv_update_context::deequipped) {
            emit send_deequipped(linked_inventory->get_item(slot));
            if (slot < lower_boundry || slot > upper_boundry)
                return;
            item_objects[slot - lower_boundry]->set_deequipped();
            return;
        }
        if (context_ == inv_update_context::removed_item) {
            if (slot > upper_boundry)
                return;
            if (slot < lower_boundry) {
                for (size_t i = 1; i < item_objects.size(); ++i) {
                    layout->removeWidget(item_objects[i]->_disp);
                    layout->replaceWidget(item_objects[0]->_disp, item_objects[i]->_disp);
                    layout->addWidget(item_objects[0]->_disp, i / _cols, i % _cols);
                }
                delete item_objects[0];
                item_objects.erase(item_objects.begin());
                if (linked_inventory->get_items_size() > upper_boundry) {
                    append_object(upper_boundry);
                }
                this->shrink_widget_to_contents(lower_boundry);
                return;
            }
            if (slot >= lower_boundry && slot <= upper_boundry) {
                slot = slot - lower_boundry;
                for (size_t i = slot + 1; i < item_objects.size(); ++i) {
                    layout->removeWidget(item_objects[i]->_disp);
                    layout->replaceWidget(item_objects[slot]->_disp, item_objects[i]->_disp);
                    layout->addWidget(item_objects[slot]->_disp, i / _cols, i % _cols);
                }
                delete item_objects[slot];
                item_objects.erase(item_objects.begin() + slot);
                if (linked_inventory->get_items_size() > upper_boundry) {
                    append_object(upper_boundry);
                }
                this->shrink_widget_to_contents(lower_boundry);
                return;
            }
        }
        if (context_ == inv_update_context::refresh_stack) {
            if (slot > upper_boundry || slot < lower_boundry)
                return;

            item_objects[slot - lower_boundry]->refresh_stack();
        }
        if (context_ == inv_update_context::added_item) {
            if (slot > upper_boundry)
                return;
            if (slot < lower_boundry) {
                size_t final_object = item_objects.size() - 1;
                for (size_t i = final_object - 1; i >= 0; --i) {
                    layout->removeWidget(item_objects[i]->_disp);
                    layout->replaceWidget(item_objects[final_object]->_disp, item_objects[i]->_disp);
                    layout->addWidget(item_objects[final_object]->_disp, i / _cols, i % _cols);
                }
                layout->removeWidget(item_objects[final_object]->_disp);
                if (final_object == upper_boundry) {
                    delete item_objects[final_object];
                    item_objects.erase(item_objects.end());
                    item_objects.shrink_to_fit();
                } else {
                    layout->addWidget(item_objects[final_object]->_disp, (final_object + 1) / _cols, (final_object + 1) % _cols);
                }
                prepend_object(lower_boundry);
                this->shrink_widget_to_contents(lower_boundry);
                return;
            }
            if (slot >= lower_boundry && slot <= upper_boundry) {
                size_t final_object = item_objects.size() - 1;
                for (size_t i = final_object - 1; i >= slot; --i) {
                    layout->removeWidget(item_objects[i]->_disp);
                    layout->replaceWidget(item_objects[final_object]->_disp, item_objects[i]->_disp);
                    layout->addWidget(item_objects[final_object]->_disp, i / _cols, i % _cols);
                }
                layout->removeWidget(item_objects[final_object]->_disp);
                if (final_object == upper_boundry) {
                    delete item_objects[final_object];
                    item_objects.erase(item_objects.end() - 1);
                    item_objects.shrink_to_fit();
                } else {
                    layout->addWidget(item_objects[final_object]->_disp, final_object / _cols, final_object % _cols);
                }
                append_object(slot);
                this->shrink_widget_to_contents(lower_boundry);
                return;
            }
        }
    }
    void scroll(int delta) {
        if (linked_inventory->get_items_size() == 0)
            return;

        if (item_objects.size() == 0) {
            _scrolled_cols = linked_inventory->get_items_size() / _cols;
        } else {
            _scrolled_cols = linked_inventory->get_slot(item_objects[0]->linked_item) / _cols;
        }
        if (delta > 0) {
            if (_scrolled_cols == 0)
                return;
            --_scrolled_cols;
            unsigned int lower_boundry = _scrolled_cols * _cols;
            unsigned int final_object = _cols * (_displayed_rows) - 1;
            size_t original_size = item_objects.size();
            if (original_size <= final_object) {
                item_objects.reserve(original_size + (final_object - original_size + 1));
                for (unsigned int i = item_objects.size(); i <= final_object; ++i) {
                    item_object* dummy = new item_object();
                    dummy->_disp = new tracked_button();
                    layout->addWidget(dummy->_disp, i / _cols, i % _cols);
                    item_objects.emplace_back(dummy);
                }
            }
            for (unsigned int i = _cols * (_displayed_rows) - 1; i >= _cols; --i) {
                layout->removeWidget(item_objects[i]->_disp);
                layout->replaceWidget(item_objects[i % _cols]->_disp, item_objects[i]->_disp);
                layout->addWidget(item_objects[i % _cols]->_disp, i / _cols, i % _cols);
            }
            std::vector<item_object*> temp = {};
            for (unsigned int j = 0; j < _cols; ++j) {
                unsigned int to_replace = item_objects.size() - _cols + j;
                item_object* obj = create_object(linked_inventory->get_item(lower_boundry + j));
                temp.emplace_back(obj);
                layout->replaceWidget(item_objects[to_replace]->_disp, obj->_disp);
                delete item_objects[to_replace];
                item_objects.erase(item_objects.begin() + to_replace);
            }
            for (unsigned int k = item_objects.size() - 1; k >= original_size && k < item_objects.size(); --k) {
                delete item_objects[k];
                item_objects.erase(item_objects.end() - 1);
            }

            item_objects.insert(item_objects.begin(), temp.begin(), temp.end());
            this->shrink_widget_to_contents(lower_boundry);
            return;
        }
        if (delta < 0) {
            if (((long long)(linked_inventory->get_items_size()) - (_scrolled_cols * _cols)) <= _displayed_rows * _cols)
                return;

            ++_scrolled_cols;
            unsigned int lower_boundry = _scrolled_cols * _cols;
            for (unsigned int i = _cols; i < item_objects.size(); ++i) {
                layout->removeWidget(item_objects[i]->_disp);
                layout->replaceWidget(item_objects[i % _cols]->_disp, item_objects[i]->_disp);
                layout->addWidget(item_objects[i % _cols]->_disp, i / _cols, i % _cols);
            }
            for (unsigned int j = _cols * (_displayed_rows - 1); j < _cols * _displayed_rows; ++j) {

                if (linked_inventory->get_items().size() > lower_boundry + j) {
                    item_object* obj = create_object(linked_inventory->get_item(lower_boundry + j));
                    layout->replaceWidget(item_objects[0]->_disp, obj->_disp);
                    item_objects.emplace_back(obj);
                    delete item_objects[0];
                    item_objects.erase(item_objects.begin());
                    continue;
                }
                layout->removeWidget(item_objects[0]->_disp);
                delete item_objects[0];
                item_objects.erase(item_objects.begin());
            }
            return;
        }
    }
protected:
    unsigned short _cols;
    unsigned short _displayed_rows;
    unsigned short _scrolled_cols = 0;
    unsigned short _item_size;
    void shrink_widget_to_contents(unsigned int lower_boundry) {
        unsigned int inventory_size = linked_inventory->get_items_size() - lower_boundry;
        unsigned int widget_size_x = _item_size * (inventory_size < _cols ? inventory_size : _cols);
        if (inventory_size % _cols == 0) {
            --inventory_size;
        }
        unsigned int inventory_size_div_col = (inventory_size / _cols) + 1;
        unsigned int widget_size_y = ((inventory_size_div_col > _displayed_rows ? _displayed_rows : inventory_size_div_col)) * _item_size;
        layout_widget->setGeometry(QRect(QPoint(0,0), QSize(widget_size_x, widget_size_y)));
    }
    item_object* create_object(item* link_item) {
        item_object* itm_obj = new item_object(link_item, QPoint(0,0), QSize(_item_size, _item_size));
        itm_obj->_disp->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        connect(itm_obj, &item_object::click_send_to_parent, this, [=]() {this->process_item_click(itm_obj);});
        if (linked_inventory->is_equipped(link_item)) {
            itm_obj->set_equipped();
        }
        return itm_obj;
    }
    void append_object(unsigned int index) {
        item_object* itm_obj = create_object(linked_inventory->get_item(index));
        index = index - (_scrolled_cols * _cols);
        item_objects.emplace_back(itm_obj);
        layout->addWidget(item_objects[index]->_disp, index / _cols, index % _cols);
    }
    void prepend_object(unsigned int index) {
        item_object* itm_obj = new item_object(linked_inventory->get_item(index), QPoint(0,0), QSize(_item_size, _item_size));
        itm_obj->_disp->setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        connect(itm_obj, &item_object::click_send_to_parent, this, [=]() {this->process_item_click(itm_obj);});
        if (linked_inventory->is_equipped(itm_obj->linked_item)) {
            itm_obj->set_equipped();
        }
        item_objects.insert(item_objects.begin(), itm_obj);
        layout->addWidget(item_objects[index]->_disp, 0, 0);
    }

public:
    inventory_context context;
    inventory* linked_inventory;
    inventory_background* base;
    QWidget* layout_widget;
    QGridLayout* layout;
    std::vector<item_object*> item_objects;
    inventory_object() = default;
    inventory_object(inventory* link_inventory, inventory_context inventory_context_ = inventory_context::container_self, unsigned short columns = 3, unsigned short rows = 10, unsigned int item_size = 150, const QPoint& coord = QPoint(0,0), QWidget* parent = nullptr): QWidget(parent) {
        _cols = columns;
        _displayed_rows = rows;
        _item_size = item_size;
        context = inventory_context_;
        linked_inventory = link_inventory;
        base = new inventory_background("inventory_background_tile",this);
        this->setGeometry(QRect(coord, QSize(_cols*_item_size, _displayed_rows*_item_size)));
        base->setGeometry(0, 0, this->width(), this->height());
        base->sprite = base->sprite.scaled(_item_size, _item_size);
        base->repaint();
        layout_widget = new QWidget(base);
        this->shrink_widget_to_contents(0);
        layout = new QGridLayout(layout_widget);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        for (unsigned int i = 0; i < _cols * _displayed_rows && i < linked_inventory->get_items_size(); ++i) {
            append_object(i);
        }
        connect(linked_inventory, &inventory::trigger_update, this, &inventory_object::update);
        connect(base, &inventory_background::scrolled, this, &inventory_object::scroll);
    }
    ~inventory_object() {
        base->deleteLater();
    }
};

class player_inventory_widget: public QWidget, public can_play_sound {

    Q_OBJECT

public slots:
    void process_click(item_object* obj) {
        if (obj->linked_item == nullptr) {
            return;
        }
        if (obj->_disp->linked_tooltip != nullptr) {
            obj->_disp->linked_tooltip->deleteLater();
        }
        auto overlay = new tracked_button(&global::w);
        overlay->setGeometry(0, 0, global::window_width, global::window_height);
        overlay->setStyleSheet(QString("tracked_button {border-image: url(assets:/pictures/null.png);}"));

        QPoint button_pos = QCursor::pos();
        QSize button_size = QSize(150, 30);
        QPoint button_increment = QPoint(0, button_size.height());
        if (obj->linked_item->is_ammo_type() || obj->linked_item->is_armor_type() || obj->linked_item->is_consumable_type() || obj->linked_item->is_weapon_type()) {
            if (!player_inventory->linked_inventory->is_equipped(obj->linked_item)) {
                auto equip_b = new QPushButton("Экипировать", overlay);
                equip_b->setGeometry(QRect(button_pos, button_size));
                connect(equip_b, &QPushButton::clicked, this, [=]() {player_inventory->linked_inventory->equip(obj->linked_item); (player_inventory->linked_inventory->is_equipped(obj->linked_item)? this->play_sfx(sfx_equip) : this->play_sfx(sfx_fail)); overlay->deleteLater();});
            } else {
                auto deequip_b = new QPushButton("Убрать", overlay);
                deequip_b->setGeometry(QRect(button_pos, button_size));
                connect(deequip_b, &QPushButton::clicked, this, [=]() {if (obj->linked_item->is_weapon_type() && player_inventory->linked_inventory->get_equipped_ammo() != nullptr) {player_inventory->linked_inventory->deequip(player_inventory->linked_inventory->get_equipped_ammo());} player_inventory->linked_inventory->deequip(obj->linked_item); this->play_sfx(sfx_deequip); overlay->deleteLater();});
            }
            button_pos += button_increment;
        }
        if (obj->linked_item->get_stack() == 1) {
            auto throw_away_b = new QPushButton("Выбросить", overlay);
            throw_away_b->setGeometry(QRect(button_pos, button_size));
            connect(throw_away_b, &QPushButton::clicked, this, [=]() {if (obj->linked_item->is_weapon_type() && player_inventory->linked_inventory->get_equipped_ammo() != nullptr) {player_inventory->linked_inventory->deequip(player_inventory->linked_inventory->get_equipped_ammo());} player_inventory->linked_inventory->remove_item(player_inventory->linked_inventory->get_slot(obj->linked_item)); this->play_sfx(sfx_throw); overlay->deleteLater();});
            button_pos += button_increment;
        } else if (obj->linked_item->get_stack() > 1) {
            auto throw_away_b = new QPushButton("Выбросить всё", overlay);
            throw_away_b->setGeometry(QRect(button_pos, button_size));
            connect(throw_away_b, &QPushButton::clicked, this, [=]() {if (obj->linked_item->is_weapon_type() && player_inventory->linked_inventory->get_equipped_ammo() != nullptr) {player_inventory->linked_inventory->deequip(player_inventory->linked_inventory->get_equipped_ammo());} player_inventory->linked_inventory->remove_item(player_inventory->linked_inventory->get_slot(obj->linked_item)); this->play_sfx(sfx_throw_all); overlay->deleteLater();});
            button_pos += button_increment;
            auto throw_away_some_b = new QPushButton("Выбросить 1 шт.", overlay);
            throw_away_some_b->setGeometry(QRect(button_pos, button_size));
            button_pos += button_increment;
            auto throw_away_some_s_bg = new QPushButton(overlay);
            throw_away_some_s_bg->setGeometry(QRect(button_pos, button_size));
            auto throw_away_some_s = new QSlider(Qt::Horizontal, throw_away_some_s_bg);
            throw_away_some_s->resize(button_size);
            throw_away_some_s->setStyleSheet(QString("QSlider::groove:horizonal {height: 6px; background: black; border-radius: 3px;}" "QSlider::handle:horizonal {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #fff, stop:1 #dedede); border: 1px solid #ababab; width: 12px; height: 20px; margin: -12px 0; border-radius: 3px;}"));
            throw_away_some_s->setMaximum(obj->linked_item->get_stack());
            throw_away_some_s->setMinimum(1);
            throw_away_some_s->setValue(1);
            connect(throw_away_some_s, &QSlider::valueChanged, this, [=](int value) {throw_away_some_b->setText(QString("Выбросить %1 шт.").arg(value));});
            connect(throw_away_some_b, &QPushButton::clicked, this, [=]() {player_inventory->linked_inventory->remove_item(player_inventory->linked_inventory->get_slot(obj->linked_item), throw_away_some_s->value()); (throw_away_some_s->value() == throw_away_some_s->maximum()? this->play_sfx(sfx_throw_all) : this->play_sfx(sfx_throw_some)); overlay->deleteLater();});
            button_pos += button_increment;
        }
        auto cancel_button = new QPushButton("Отмена", overlay);
        cancel_button->setGeometry(QRect(button_pos, button_size));
        connect(cancel_button, &QPushButton::clicked, this, [=]() {overlay->deleteLater();});
        connect(overlay, &tracked_button::clicked, this, [=]() {overlay->deleteLater();});
        overlay->show();
    }

    void update_weight() {
        weight_display->setText(QString("Вес: %1/%2").arg(global::player_->get_weight()).arg(global::player_->get_max_weight()));
        if (global::player_->get_weight() > global::player_->get_max_weight()) {
            weight_display->setStyleSheet("color: rgb(200, 0, 0);");
        } else {
            weight_display->setStyleSheet("color: rgb(255, 255, 255);");
        }
    }
    void equip_item_in_slot(item* item_) {
        if (item_ == nullptr)
            return;

        if (item_->is_armor_type()) {
            switch(static_cast<armor*>(item_)->get_armor_slot()) {
            case armor_slot::head: {
                change_slot(armor_head_slot, item_);
                return;
            }
            case armor_slot::body: {
                change_slot(armor_body_slot, item_);
                return;
            }
            case armor_slot::legs: {
                change_slot(armor_legs_slot, item_);
                return;
            }
            }
        }
        if (item_->is_weapon_type()) {
            change_slot(weapon_slot, item_);
            return;
        }
        if (item_->is_ammo_type()) {
            change_slot(ammo_slot, item_);
            return;
        }
        if (item_->is_consumable_type()) {
            change_slot(consumable_slot, item_);
            return;
        }
    }
    void deequip_item_in_slot(item* item_) {
        if (item_ == nullptr)
            return;

        if (item_->is_armor_type()) {
            switch(static_cast<armor*>(item_)->get_armor_slot()) {
            case armor_slot::head: {
                change_slot(armor_head_slot, nullptr);
                return;
            }
            case armor_slot::body: {
                change_slot(armor_body_slot, nullptr);
                return;
            }
            case armor_slot::legs: {
                change_slot(armor_legs_slot, nullptr);
                return;
            }
            }
        }
        if (item_->is_weapon_type()) {
            change_slot(weapon_slot, nullptr);
            return;
        }
        if (item_->is_ammo_type()) {
            change_slot(ammo_slot, nullptr);
            return;
        }
        if (item_->is_consumable_type()) {
            change_slot(consumable_slot, nullptr);
            return;
        }
    }
protected:
    void change_slot(item_object*& slot_, item* item_) {
        slot_->change_item(item_);
    }
    void paintEvent(QPaintEvent *event) {
        QPainter paint(this);
        if (sprite.isNull())
            return;

        paint.drawPixmap(this->rect(), sprite);
    }
public:
    QPixmap sprite;
    QSoundEffect* sfx_equip = nullptr;
    QSoundEffect* sfx_deequip = nullptr;
    QSoundEffect* sfx_throw_some = nullptr;
    QSoundEffect* sfx_throw_all = nullptr;
    QSoundEffect* sfx_throw = nullptr;
    QSoundEffect* sfx_fail = nullptr;
    inventory_object* player_inventory = nullptr;
    item_object* armor_head_slot = nullptr;
    item_object* armor_body_slot = nullptr;
    item_object* armor_legs_slot = nullptr;
    item_object* weapon_slot = nullptr;
    item_object* ammo_slot = nullptr;
    item_object* consumable_slot = nullptr;
    QLabel* weight_display = nullptr;
    player_inventory_widget(QWidget* parent = nullptr): QWidget(parent) {
        sfx_equip = new QSoundEffect(this);
        sfx_equip->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/equip.wav")).absoluteFilePath()));
        sfx_deequip = new QSoundEffect(this);
        sfx_deequip->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/deequip.wav")).absoluteFilePath()));
        sfx_throw_some = new QSoundEffect(this);
        sfx_throw_some->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/throw_some.wav")).absoluteFilePath()));
        sfx_throw_all = new QSoundEffect(this);
        sfx_throw_all->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/throw_all.wav")).absoluteFilePath()));
        sfx_throw = new QSoundEffect(this);
        sfx_throw->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/throw.wav")).absoluteFilePath()));
        sfx_fail = new QSoundEffect(this);
        sfx_fail->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/fail.wav")).absoluteFilePath()));
        this->resize(1425, 900);
        sprite.load("assets:/pictures/black.png");
        player_inventory = new inventory_object(global::player_->get_inventory(), inventory_context::player_inventory, 4, 6, 125, QPoint(70,70), this);
        connect(player_inventory, &inventory_object::send_equipped, this, &player_inventory_widget::equip_item_in_slot);
        connect(player_inventory, &inventory_object::send_deequipped, this, &player_inventory_widget::deequip_item_in_slot);
        armor_head_slot = new item_object(player_inventory->linked_inventory->get_armor_head(), QPoint(1170, 70), QSize(125,125));
        armor_head_slot->_disp->setParent(this);
        armor_body_slot = new item_object(player_inventory->linked_inventory->get_armor_body(), QPoint(1170, 195), QSize(125,125));
        armor_body_slot->_disp->setParent(this);
        armor_legs_slot = new item_object(player_inventory->linked_inventory->get_armor_legs(), QPoint(1170, 320), QSize(125,125));
        armor_legs_slot->_disp->setParent(this);
        weapon_slot = new item_object(player_inventory->linked_inventory->get_weapon(), QPoint(920, 450), QSize(125,125));
        weapon_slot->_disp->setParent(this);
        ammo_slot = new item_object(player_inventory->linked_inventory->get_equipped_ammo(), QPoint(920, 575), QSize(125,125));
        ammo_slot->_disp->setParent(this);
        consumable_slot = new item_object(player_inventory->linked_inventory->get_equipped_consumable(), QPoint(1075, 450), QSize(125,125));
        consumable_slot->_disp->setParent(this);
        weight_display = new QLabel(this);
        weight_display->setGeometry(10,10, 300, 20);
        update_weight();
        connect(armor_head_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(armor_body_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(armor_legs_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(weapon_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(ammo_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(consumable_slot, &item_object::click_send_to_parent, this, &player_inventory_widget::process_click);
        connect(player_inventory->linked_inventory, &inventory::change_weight, this, &player_inventory_widget::update_weight);
        connect(player_inventory, &inventory_object::send_click, this, &player_inventory_widget::process_click);
    }
};

class entity_object: public animated_displayable {

    Q_OBJECT
private:
    entity* linked_entity;
public slots:
    void markdown_entity() {
        if (_disp->linked_tooltip == nullptr)
            return;

        _disp->linked_tooltip->setText(QString("<center><font size=\"4\">%1</font></center>").arg(linked_entity->get_name()));
    }
public:
    entity* get_linked_entity() {return linked_entity;}
    entity_object(entity* linked_entity_, const anim_sequence& anim_sequence_ = anim_sequence(), const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool needds_tooltip = true)
        : animated_displayable(linked_entity_->get_asset(), anim_sequence_, coord, size), linked_entity(linked_entity_)
    {

        if (needds_tooltip) {
            _disp->tooltip = tooltip_types::name_display;
            connect(_disp, &tracked_button::request_tooltip, this, &entity_object::markdown_entity);
        }

    }

};

class enemy_object : public animated_displayable {
    Q_OBJECT
private:
    enemy* _linked_enemy;
    QProgressBar* _healthbar;
public:
    enemy_object(enemy* linked_enemy_, const anim_sequence& anim_sequence_ = anim_sequence(),
                 const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), QProgressBar* healthbar = nullptr)
        : animated_displayable(linked_enemy_->get_asset(), anim_sequence_, coord, size), _linked_enemy(linked_enemy_), _healthbar(healthbar) {};
    enemy* get_linked_enemy() {return _linked_enemy;}
    QProgressBar* get_healthbar() {return _healthbar;}
public slots:
    void enemy_clicked();
};

class text_object : public QLabel {

    Q_OBJECT

signals:
    void request_new_string();
public slots:
    void tick() {
        ++_ticks_passed;
        if (_ticks_passed < ticks_per_symbol)
            return;

        _ticks_passed = 0;
        this->write();
    }
    void mouseReleaseEvent(QMouseEvent *ev) {
        ev->ignore();
    }
    void process_click() {
        if (_currently_writing = true) {
            this->final();
            return;
        }
        emit request_new_string();
    }
protected:
    QString _processed_string;
    QSoundEffect* _sfx = nullptr;
    unsigned int _ticks_passed = 0;
    unsigned int _pause_for = 0;
    bool _currently_writing = false;
    void write() {
        if (_processed_string.size() != text_source.size() && no_animation == false) {
            while (_processed_string.size() < text_source.size() - 1 && text_source[_processed_string.size()] == " ") {
                _processed_string += text_source[_processed_string.size()];
            }
            _processed_string += text_source[_processed_string.size()];
            this->setText(_processed_string);
        } else {
            this->final();
            return;
        }
        this->play_sound();
    }
    void play_sound() {
        if (_sfx != nullptr) {
            _sfx->stop();
            _sfx->setVolume(global::master_volume * global::sfx_volume);
            _sfx->play();
        }
    }
    void final() {
        _processed_string = text_source; /*+ QString(" <img src=\"assets:/pictures/ui_text_go_next.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\">");*/
        this->setText(_processed_string);
        disconnect(global::timer, &QTimer::timeout, this, &text_object::tick);
        _currently_writing = false;
    }
public:
    QString text_source;
    QString symbol_sound;
    unsigned int ticks_per_symbol;
    bool no_animation;
    text_object(QString text_source_, QString symbol_sound_ = "", bool no_animation_ = false, unsigned int ticks_per_symbol_ = 1, QWidget* parent = &global::w): QLabel(parent), symbol_sound(symbol_sound_), no_animation(no_animation_), ticks_per_symbol(ticks_per_symbol_) {
        this->setWordWrap(true);
        this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        if (symbol_sound.isEmpty() == false) {
            _sfx = new QSoundEffect(this);
            _sfx->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/%1.wav").arg(symbol_sound)).absoluteFilePath()));
        }
        if (text_source_.isEmpty()) {
            return;
        }
        this->new_text_source(text_source_);
    }
    void new_text_source(QString text_source_) {
        text_source = text_source_;
        if (no_animation == true) {
            this->final();
            this->play_sound();
            return;
        }
        _processed_string = "";
        connect(global::timer, &QTimer::timeout, this, &text_object::tick);
        _currently_writing = true;
    }
    ~text_object() {
        delete _sfx;
    }
};

class text_holder: public tracked_button {

    Q_OBJECT

protected:
    text_object* _linked_text_object = nullptr;
public:
    QPixmap sprite;
    text_holder(const QString& asset, QSize size = QSize(1920,300), QPoint point = QPoint(0,0), QWidget* parent = &global::w): tracked_button(parent) {
        sprite.load(QString("assets:/pictures/%1.png").arg(asset));
        this->setGeometry(QRect(point, size));
    }
    text_holder(const QString& asset, text_object* linked_text_object, QSize size = QSize(1920,300), QPoint point = QPoint(0,0), QWidget* parent = &global::w): text_holder(asset, size, point, parent) {
        set_text_object(linked_text_object);
    }
    void set_text_object(text_object* new_text_object) {
        delete _linked_text_object;
        _linked_text_object = new_text_object;
        _linked_text_object->setParent(this);
        _linked_text_object->setGeometry(this->rect());
        connect(this, &text_holder::clicked, _linked_text_object, &text_object::process_click);
    }
    void paintEvent(QPaintEvent *event) {
        QPainter paint(this);
        if (sprite.isNull())
            return;

        paint.drawPixmap(this->rect(), sprite);
    }
    ~text_holder() {
        delete _linked_text_object;
    }
};

class map_player_object: public animated_displayable, public can_play_sound {

    Q_OBJECT
signals:
    void check_tiles(int x, int y, short width, short height, bool& locked);
    void link_line(QGraphicsLineItem*& line);
    void cant_move_there(QPoint& coord);
    void call_parent(QWidget*& marker_parent);
public slots:
    void stop_running() {
        this->set_current_anim(0);
        move_player_marker();
        player_marker->show();
        destination_marker->hide();
        delete line;
        line = nullptr;
    }
    void begin_running() {
        if (this->get_anim_sequence().current_anim_id != 1) {
            this->set_current_anim(1);
        }
        player_marker->hide();
        move_destination_marker();
        destination_marker->show();
        delete line;
        link_line(line);
    }
public:
    QSoundEffect* sfx_fail = nullptr;
    unclickable_button* player_marker = nullptr;
    unclickable_button* destination_marker = nullptr;
    QGraphicsLineItem* line = nullptr;
    map_player_object(QWidget* parent): animated_displayable("map_player", anim_sequence(0, anim("static", 1, 0), anim("moving", 20, 3)), QPoint(0,0), QSize(13,24), false) {
        if (parent == nullptr) {
            delete this;
            return;
        }

        sfx_fail = new QSoundEffect(this);
        sfx_fail->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/fail.wav")).absoluteFilePath()));

        this->_disp->setParent(parent);
        connect(this, &map_player_object::began_step, this, &map_player_object::begin_running);
        connect(this, &map_player_object::reached_destination, this, &map_player_object::stop_running);
        QWidget* marker_parent = parent->parentWidget();
        player_marker = new unclickable_button(marker_parent);
        player_marker->setObjectName("player_marker");
        player_marker->setGeometry(QRect(0, 0, 32, 32));
        player_marker->setStyleSheet(QString("border-image: url(assets:/pictures/map_player_marker.png)"));
        move_player_marker();
        player_marker->show();
        destination_marker = new unclickable_button(marker_parent);
        destination_marker->setGeometry(QRect(0, 0, 32, 32));
        destination_marker->setStyleSheet(QString("border-image: url(assets:/pictures/map_player_destination.png)"));
        destination_marker->hide();
    }
    void move_to(QPoint& coord) override {
        if (global::player_->get_weight() > global::player_->get_max_weight()) {
            interrupt();
            new screen_message("> Вы перегружены и не можете идти", 50, 200, 20);
            play_sfx(sfx_fail);
            return;
        }
        bool locked = false;
        emit check_tiles(coord.x(), coord.y(), this->_disp->size().width(), this->_disp->size().height(), locked);
        if (locked == true) {
            this->interrupt();
            emit cant_move_there(coord);
            return;
        }
        _disp->move(coord);
        if (line != nullptr) {
            line->setLine(coord.x() + this->_disp->width() / 2, coord.y() + this->_disp->height() / 2, this->get_transpos().final_destination.x() + this->_disp->width() / 2,this->get_transpos().final_destination.y() + this->_disp->height() / 2);
        }
    }
    void move_player_marker() {
        int dx = (player_marker->width() / 2) - (this->_disp->width() / 2);
        int dy = (player_marker->height() / 2) + (this->_disp->height() / 2) + 3;
        dx -= (this->_disp->width() / 2);
        player_marker->move(this->_disp->mapToGlobal(this->_disp->pos() - this->_disp->geometry().center()) - QPoint(dx, dy));
    }
    void move_destination_marker() {
        destination_marker->move(this->_disp->parentWidget()->mapToGlobal(this->get_transpos().final_destination) - QPoint(this->destination_marker->width() / 4, 0));
    }
};

class map_poi: public displayable {

    Q_OBJECT

protected:
    unsigned long long _location_id;
    QString _location_name;
public slots:
    void markdown_poi() {
        if (_disp->linked_tooltip == nullptr)
            return;


        _disp->linked_tooltip->setText(QString("<center><font size=\"4\">%1</font></center>").arg(_location_name));
    }
public:
    map_poi(const QString& asset, unsigned long long location_id, const QString& location_name = "?"): displayable(asset, QPoint(0,0), QSize(75,75)), _location_id(location_id), _location_name(location_name)
    {
        _disp->tooltip = tooltip_types::location_on_map;
        connect(_disp, &tracked_button::request_tooltip, this, &map_poi::markdown_poi);
    }
    void set_name(const QString& name) {
        _location_name = name;
    }
    void set_location_id(unsigned long long id) {
        _location_id = id;
    }
    QString get_name() {
        return _location_name;
    }
    unsigned long long get_location_id() {
        return _location_id;
    }
};

class map_grid_tile: public tracked_button {
    Q_OBJECT
protected:
    bool _is_locked = true;
    biome _biome = biome::none;
    map_poi* _poi = nullptr;
    float _difficulty = 0;
signals:
    void clicked_tile(map_grid_tile* tile);
    void clicked_poi(map_grid_tile* tile);
    void clicked_locked_tile(map_grid_tile* tile);
public slots:
    void process_click() {
        if (this->_is_locked == true) {
            emit clicked_locked_tile(this);
            return;
        }
        emit clicked_tile(this);
    }
    void process_click_poi() {
        emit clicked_poi(this);
    }
public:
    map_grid_tile() = default;
    map_grid_tile(unsigned short size, float difficulty = 0.f, biome biome_ = biome::none, bool is_locked = true, map_poi* poi = nullptr) {
        setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Policy::Ignored);
        _is_locked = is_locked;
        _biome = biome_;
        _difficulty = difficulty;
        this->setGeometry(0, 0, size, size);
        connect(this, &QPushButton::clicked, this, &map_grid_tile::process_click);
        if (_is_locked == true) {
            this->setStyleSheet(QString("border-image: url(assets:/pictures/black.png);"));
        } else {
            this->setStyleSheet(QString("border-image: url(assets:/pictures/null.png);"));
        }
        if (poi == nullptr)
            return;

        set_poi(poi);
    }
    void unlock() {
        _is_locked = false;
        this->setStyleSheet(QString("border-image: url(assets:/pictures/null.png);"));
        if (_poi != nullptr) {
            _poi->_disp->show();
        }
    }
    void lock() {
        _is_locked = true;
        this->setStyleSheet(QString("border-image: url(assets:/pictures/black.png);"));
        if (_poi != nullptr) {
            _poi->_disp->hide();
        }
    }
    void set_poi(map_poi* poi_) {
        delete _poi;
        _poi = poi_;
        _poi->setParent(this);
        _poi->_disp->setParent(this);
        connect(poi_->_disp, &QPushButton::clicked, this, &map_grid_tile::process_click_poi);
        if (_is_locked == true)
            _poi->_disp->hide();
    }
    void set_biome(biome biome_) {
        _biome = biome_;
    }
    void set_difficulty(float difficulty) {
        _difficulty = difficulty;
    }
    map_poi* get_poi() {
        return _poi;
    }
    bool get_locked() {
        return _is_locked;
    }
    biome get_biome() {
        return _biome;
    }
    float get_difficulty() {
        return _difficulty;
    }
    ~map_grid_tile() {
        delete _poi;
    }
};

class map_grid: public QWidget {

    Q_OBJECT

protected:
    unsigned short _width = 19;
    unsigned short _height = 12;
    unsigned short _tile_size = 50;
signals:
    void clicked_child_tile(map_grid_tile* tile, QPoint p);
    void clicked_child_poi(map_grid_tile* tile);
    void clicked_child_locked_tile(map_grid_tile* tile);
public slots:
    void process_clicked_tile(map_grid_tile* tile) {
        emit clicked_child_tile(tile, QPoint());
    }
    void process_clicked_poi(map_grid_tile* tile) {
        emit clicked_child_poi(tile);
    }
    void process_clicked_locked_tile(map_grid_tile* tile) {
        emit clicked_child_locked_tile(tile);
    }
public:
    QGridLayout* layout;
    std::vector<map_grid_tile*> tiles;
    map_grid(unsigned short width, unsigned short height, unsigned short tile_size = 50, QWidget* parent = nullptr): QWidget(parent) {
        _width = width;
        _height = height;
        _tile_size = tile_size;
        this->setGeometry(0, 0, _width * _tile_size, _height * _tile_size);
        layout = new QGridLayout(this);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        tiles.reserve(_width * _height);
        for (unsigned int i = 0; i < _width * _height; ++i) {
            auto tile = new map_grid_tile(_tile_size);
            tiles.emplace_back(tile);
            layout->addWidget(tile, i / _width, i % _width);
            connect(tile, &map_grid_tile::clicked_tile, this, &map_grid::process_clicked_tile);
            connect(tile, &map_grid_tile::clicked_locked_tile, this, &map_grid::process_clicked_locked_tile);
            connect(tile, &map_grid_tile::clicked_poi, this, &map_grid::process_clicked_poi);
        }
    }
    map_grid_tile* get_tile_at(QPoint coord) {
        unsigned int row = coord.y() / _tile_size;
        unsigned int col = coord.x() / _tile_size;
        unsigned int index = (row * _width) + col;
        if (row >= _height || row < 0 || col >= _width || col < 0)
            return nullptr;

        return tiles[index];
    }
    ~map_grid() {
        for (auto elem : tiles) {
            elem->deleteLater();
        }
    }
};

class map_widget: public game_scene, public can_play_sound {

    Q_OBJECT

public slots:
    void clicked_tile(map_grid_tile* tile, QPoint clicked_at) {
        QPoint start = player_object->_disp->pos();
        QPoint end;
        if (clicked_at.isNull()) {
            end = this->mapFromGlobal(QCursor::pos()) - QPoint(player_object->_disp->width() / 2, player_object->_disp->height() / 2);
        } else {
            end = clicked_at - QPoint(player_object->_disp->width() / 2, player_object->_disp->height() / 2);
            if (start == end) {
                enter_poi_intent = tile;
                try_trade();
                return;
            }
        }
        enter_poi_intent = nullptr;

        if (start == end)
            return;

        QPoint vec = end - start;
        unsigned int length = std::sqrtl(QPoint::dotProduct(vec, vec));
        short survival_skill = global::player_->get_total_stats().survival;
        short agility_char = 15 - global::player_->get_total_stats().agility;
        if (survival_skill <= 9)
            survival_skill = 10;
        if (agility_char <= 0) {
            agility_char = 1;
        }
        unsigned int steps = 25 * length * agility_char / survival_skill;
        player_object->begin_step(end, steps, transpos_algs::linear);
    }
    void clicked_locked_tile(map_grid_tile* tile) {
        // tile->unlock();
        clicked_tile(tile, QPoint());
    }
    void clicked_poi(map_grid_tile* tile) {
        clicked_tile(tile, tile->geometry().center());
        enter_poi_intent = tile;
    }
    void allow_tiles(int x, int y, short width, short height, bool& locked) {
        map_grid_tile* tile1 = grid->get_tile_at(QPoint(x, y));
        map_grid_tile* tile2 = grid->get_tile_at(QPoint(x + width, y));
        map_grid_tile* tile3 = grid->get_tile_at(QPoint(x, y + height));
        map_grid_tile* tile4 = grid->get_tile_at(QPoint(x + width, y + height));
        if (tile1 == nullptr || tile2 == nullptr || tile3 == nullptr || tile4 == nullptr) {
            locked = true;
            return;
        }
        if (tile1->get_locked()) {
            tile1->unlock();
        }
        if (tile2->get_locked()) {
            tile2->unlock();
        }
        if (tile3->get_locked()) {
            tile3->unlock();
        }
        if (tile4->get_locked()) {
            tile4->unlock();
        }
        // locked = (tile1->get_locked() || tile2->get_locked() || tile3->get_locked() || tile4->get_locked());
    }

    void try_trade() {
        if (enter_poi_intent == nullptr) {
            qInfo() << "no trade";
            return;
        } else {
            auto loc_path = global::root_path;
            loc_path /= "objects";
            auto trade_path = loc_path;
            loc_path /= "location";
            trade_path /= "trade";
            loc_path /= QString("location_%1.json").arg(enter_poi_intent->get_poi()->get_location_id()).toStdString();
            json j;
            std::ifstream file(loc_path.string());
            try {
                j = json::parse(file);
            } catch (...) {
                critical_error(QString("Не удалось загрузить локацию location_%1").arg(enter_poi_intent->get_poi()->get_location_id()));
                return;
            }
            if (j.contains("trades")) {
                if (j["trades"].is_number_unsigned()) {
                    trade_path /= QString("trade_%1.json").arg(j["trades"].get<unsigned long long>()).toStdString();
                    std::ifstream trade_file(trade_path.string());
                    json trade_j;
                    try {
                        trade_j = json::parse(trade_file);
                    } catch (...) {
                        critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                        return;
                    }
                    if (!trade_j.contains("what_trade_is_that") || !trade_j.contains("recieve")) {
                        critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                        return;
                    }
                    if (!trade_j["what_trade_is_that"].is_number_unsigned() || !trade_j["recieve"].is_number_unsigned()) {
                        critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                        return;
                    }
                    switch (trade_j["what_trade_is_that"].get<int>()) {
                    case 0: {
                        item* item_ = item_from_id(trade_j["recieve"].get<int>());
                        if (item_ == nullptr) {
                            critical_error(QString("Не удалось загрузить предмет item_%1").arg(trade_j["recieve"].get<int>()));
                            return;
                        }
                        if (!trade_j.contains("offer")) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        if (!trade_j["offer"].is_number_unsigned()) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        item* item_of = item_from_id(trade_j["offer"].get<int>());
                        if (item_of == nullptr) {
                            critical_error(QString("Не удалось загрузить предмет item_%1").arg(trade_j["offer"].get<int>()));
                            return;
                        }
                        auto item_to_edit = global::player_->get_inventory()->get_equal_item(item_of);
                        if (item_to_edit == nullptr) {
                            new screen_message(QString("> У вас нет предмета: %1").arg(item_of->get_name()), 50, 100, 16);
                            play_sfx(sfx_fail);
                            delete item_;
                            return;
                        }
                        global::player_->get_inventory()->remove_item(global::player_->get_inventory()->get_slot(item_to_edit), 1);
                        global::player_->get_inventory()->add_item(item_);
                        new screen_message(QString("> Вы обменяли %1 на %2").arg(item_of->get_name()).arg(item_->get_name()), 60, 30, 16);
                        play_sfx(sfx_trade_success);
                        delete item_of;
                        if (trade_j.contains("exp")) {
                            if (trade_j["exp"].is_number_unsigned()) {
                                if (trade_j["exp"].get<int>() > 0) {
                                    global::player_->add_exp(trade_j["exp"].get<int>());
                                }
                            } else {
                                critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                                return;
                            }
                        }
                        break;
                    }
                    case 1: {
                        int get_money = trade_j["recieve"].get<int>();
                        if (!trade_j.contains("offer")) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        if (!trade_j["offer"].is_number_unsigned()) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        item* item_ = item_from_id(trade_j["offer"].get<int>());
                        if (item_ == nullptr) {
                            critical_error(QString("Не удалось загрузить предмет item_%1").arg(trade_j["offer"].get<int>()));
                            return;
                        }
                        auto item_to_edit = global::player_->get_inventory()->get_equal_item(item_);
                        if (item_to_edit == nullptr) {
                            new screen_message(QString("> У вас нет предмета: %1").arg(item_->get_name()), 50, 100, 16);
                            play_sfx(sfx_fail);
                            delete item_;
                            return;
                        }
                        global::player_->get_inventory()->remove_item(global::player_->get_inventory()->get_slot(item_to_edit), 1);
                        double stat = (double)global::player_->get_entity_stats().get_stat(skill_type::barter);
                        double coef = (stat - 5) / stat;
                        int money = (item_->get_base_cost() + get_money) * coef;
                        global::player_->add_money(money);
                        new screen_message(QString("> Вы продали %1 за %2").arg(item_->get_name()).arg(money), 50, 100, 16);
                        play_sfx(sfx_trade_success);
                        if (trade_j.contains("exp")) {
                            if (trade_j["exp"].is_number_unsigned()) {
                                if (trade_j["exp"].get<int>() > 0) {
                                    global::player_->add_exp(trade_j["exp"].get<int>());
                                }
                            } else {
                                critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                                return;
                            }
                        }
                        break;
                    }
                    case 2: {
                        item* item_ = item_from_id(trade_j["recieve"].get<int>());
                        if (item_ == nullptr) {
                            critical_error(QString("Не удалось загрузить предмет item_%1").arg(trade_j["recieve"].get<int>()));
                            return;
                        }
                        if (!trade_j.contains("offer")) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        if (!trade_j["offer"].is_number_unsigned()) {
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                        int cost = trade_j["offer"].get<int>() + item_->get_base_cost();
                        double stat = (double)global::player_->get_entity_stats().get_stat(skill_type::barter);
                        double coef = 60.0 / stat;
                        cost *= coef;
                        if (global::player_->get_money() < cost) {
                            new screen_message(QString("> Вы не смогли преобрести %1 за %2").arg(item_->get_name()).arg(cost), 50, 100, 16);
                            play_sfx(sfx_fail);
                            delete item_;
                            return;
                        }
                        global::player_->set_money(global::player_->get_money() - cost);
                        global::player_->get_inventory()->add_item(item_);
                        new screen_message(QString("> Вы преобрели %1 за %2").arg(item_->get_name()).arg(cost), 60, 30, 16);
                        play_sfx(sfx_trade_success);
                        if (trade_j.contains("exp")) {
                            if (trade_j["exp"].is_number_unsigned()) {
                                if (trade_j["exp"].get<int>() > 0) {
                                    global::player_->add_exp(trade_j["exp"].get<int>());
                                }
                            } else {
                                critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                                return;
                            }
                        }
                        break;
                    }
                    case 3: {
                        try {
                        item* item_ = item_from_id(trade_j["recieve"].get<int>());
                        if (item_ == nullptr) {
                            critical_error(QString("Не удалось загрузить предмет item_%1").arg(trade_j["recieve"].get<int>()));
                            return;
                        }
                        new screen_message(QString("> Вы получили: %1").arg(item_->get_name()), 60, 30, 16);
                        play_sfx(sfx_trade_success);
                        global::player_->get_inventory()->add_item(item_);
                        if (trade_j.contains("exp")) {
                            if (trade_j["exp"].is_number_unsigned()) {
                                if (trade_j["exp"].get<int>() > 0) {
                                    global::player_->add_exp(trade_j["exp"].get<int>());
                                }
                            } else {
                                critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                                return;
                            }
                        }
                        break;
                        } catch (std::exception& e) {
                            qInfo() << e.what();
                            critical_error(QString("Не удалось загрузить сделку trade_%1").arg(j["trades"].get<unsigned long long>()));
                            return;
                        }
                    }
                    }
                } else {
                    critical_error(QString("Не удалось загрузить локацию location_%1").arg(enter_poi_intent->get_poi()->get_location_id()));
                    return;
                }
            }
        }
    }
public:
    QSoundEffect* sfx_fail = nullptr;
    QSoundEffect* sfx_trade_success = nullptr;
    map_grid_tile* enter_poi_intent = nullptr;
    map_grid* grid = nullptr;
    map_player_object* player_object = nullptr;
    map_widget(QPoint pos, QWidget* parent = nullptr): game_scene(new QGraphicsScene(), parent) {
        sfx_fail = new QSoundEffect(this);
        sfx_fail->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/fail.wav")).absoluteFilePath()));

        sfx_trade_success = new QSoundEffect(this);
        sfx_trade_success->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/trade_success.wav")).absoluteFilePath()));

        grid = new map_grid(19, 12, 75, this);
        connect(grid, &map_grid::clicked_child_tile, this, &map_widget::clicked_tile);
        connect(grid, &map_grid::clicked_child_locked_tile, this, &map_widget::clicked_locked_tile);
        connect(grid, &map_grid::clicked_child_poi, this, &map_widget::clicked_poi);
        this->setGeometry(QRect(pos,grid->rect().size()));

        setSceneRect(this->rect());
        set_background("assets:/pictures/map.jpg");
        player_object = new map_player_object(this);
        player_object->player_marker->setParent(this->parentWidget());
        player_object->destination_marker->setParent(this->parentWidget());
        connect(player_object, &map_player_object::check_tiles, this, &map_widget::allow_tiles);
        connect(player_object, &map_player_object::reached_destination, this, &map_widget::try_trade);
    }
};

class char_selector: public QWidget {

    Q_OBJECT

public slots:
    void increase_pressed() {
        emit increase(value);
        update_display();
    }
    void decrease_pressed() {
        emit decrease(value);
        update_display();
    }
signals:
    void increase(short& stat);
    void decrease(short& stat);
public:
    char_type type;
    short value = 1;
    QLabel* display = nullptr;
    tracked_button* increase_button = nullptr;
    tracked_button* decrease_button = nullptr;
    void update_display() {
        display->setText(char_type_to_str(type) + ": " + QString::number(value) + (value >= 10 ? " (max)" : ""));
    }
    char_selector(char_type type_, QWidget* parent = nullptr): QWidget(parent) {
        type = type_;
        this->setGeometry(QRect(0, 0, 250, 50));
        display = new QLabel(this);
        display->setGeometry(QRect(30, 0, 200, 50));
        display->setStyleSheet(QString("font: 16pt \"Arial\";"));
        update_display();
        increase_button = new tracked_button(this);
        increase_button->setGeometry(0, 0, 25, 25);
        increase_button->setStyleSheet(QString("border-image: url(assets:/pictures/char_increase.png);"));
        connect(increase_button, &tracked_button::clicked, this, &char_selector::increase_pressed);
        decrease_button = new tracked_button(this);
        decrease_button->setGeometry(0, 25, 25, 25);
        decrease_button->setStyleSheet(QString("border-image: url(assets:/pictures/char_decrease.png);"));
        connect(decrease_button, &tracked_button::clicked, this, &char_selector::decrease_pressed);

    }
};

class skill_selector: public QWidget {

    Q_OBJECT
public slots:
    void click(bool checked) {
        if (checked == true) {
            short old_value = value;
            emit select(value);
            if (old_value == value) {
                select_button->setChecked(false);
            }
        } else {
            emit deselect(value);
        }
        update_display();
    }
signals:
    void select(short& value);
    void deselect(short& value);
public:
    skill_type type;
    short value = 10;
    QLabel* display = nullptr;
    QCheckBox* select_button = nullptr;
    void update_display() {
        display->setText(QString::number(value));
    }
    skill_selector(skill_type type_, QWidget* parent = nullptr): QWidget(parent) {
        type = type_;
        this->setGeometry(0, 0, 200, 30);
        display = new QLabel(this);
        display->setGeometry(150, 0, 50, 30);
        display->setAlignment(Qt::AlignRight);
        update_display();
        select_button = new QCheckBox(this);
        select_button->setText(skill_type_to_str(type));
        connect(select_button, &QCheckBox::clicked, this, &skill_selector::click);
    }
};

class character_creation_widget: public QWidget {

    Q_OBJECT

signals:
    void player_created();
public slots:
    void increase_stat(short& stat) {
        if (char_points == 0 || stat >= 10) {
            return;
        }
        --char_points;
        ++stat;
        update_char_hint();
    }
    void special_increase_strength(short& stat) {
        if (char_points == 0 || stat >= 10) {
            return;
        }
        --char_points;
        ++stat;
        update_char_hint();
        modify_skill(big_guns, 3);
        modify_skill(unarmed, 3);
    }
    void special_increase_intelligence(short& stat) {
        if (char_points == 0 || stat >= 10) {
            return;
        }
        --char_points;
        ++stat;
        update_char_hint();
        modify_skill(speech, 3);
        modify_skill(science, 3);
    }
    void decrease_stat(short& stat) {
        if (stat <= 1) {
            return;
        }
        ++char_points;
        --stat;
        update_char_hint();
    }
    void special_decrease_strength(short& stat) {
        if (stat <= 1) {
            return;
        }
        ++char_points;
        --stat;
        update_char_hint();
        modify_skill(big_guns, -3);
        modify_skill(unarmed, -3);
    }
    void special_decrease_intelligence(short& stat) {
        if (stat <= 1) {
            return;
        }
        ++char_points;
        --stat;
        update_char_hint();
        modify_skill(speech, -3);
        modify_skill(science, -3);
    }
    void set_primary_skill(short& skill) {
        if (skill_points == 0) {
            return;
        }
        --skill_points;
        skill += 10;
        update_skill_hint();
    }
    void unset_primary_skill(short& skill) {
        ++skill_points;
        skill -= 10;
        update_skill_hint();
    }

    void done_clicked() {
        bool fail = false;
        if (char_points != 0) {
            fail = true;
        }

        if (skill_points != 0) {
            fail = true;
        }

        if (fail) {
            return;
        }
        entity_stats stats;

        stats.strength = strength->value;
        stats.agility = agility->value;
        stats.endurance = endurance->value;
        stats.intelligence = intelligence->value;
        stats.luck = luck->value;

        stats.guns = guns->value;
        stats.big_guns = big_guns->value;
        stats.unarmed = unarmed->value;
        stats.science = science->value;
        stats.speech = speech->value;
        stats.barter = barter->value;
        stats.survival = survival->value;

        QString player_name = name->text();
        if (player_name.isEmpty() == true) {
            player_name = "Безымянный";
        }

        global::player_ = new player();
        global::player_->set_asset(player_asset);
        global::player_->set_entity_stats(stats);
        global::player_->set_name(player_name);
        global::player_->set_inventory(new inventory());
        global::player_->set_base_stats();
        // auto x = new item_requirements();
        // x->item_requirements_ptrs.emplace_back(new char_requirement(3, char_type::intelligence));
        // armor_bonus p;
        // p.bonus = equipment_bonus::change_char_intelligence;
        // p.value = 500;
        // global::player_->get_inventory()->add_item(new armor("Шлем крутой","Очень крутой шлем", "l", 1, 1, 2.f, 50, true, x, armor_slot::head, 5, p));
        // global::player_->get_inventory()->equip(global::player_->get_inventory()->get_item(0));
        // save_player(global::player_, 1);
        // save_meta(1);

        emit player_created();
    }

    void reset_clicked() {
        name->setText(QString(""));

        strength->value = 1;
        agility->value = 1;
        endurance->value = 1;
        intelligence->value = 1;
        luck->value = 1;

        strength->update_display();
        agility->update_display();
        endurance->update_display();
        intelligence->update_display();
        luck->update_display();

        char_points = 20;
        update_char_hint();

        guns->value = 10;
        big_guns->value = 10;
        unarmed->value = 10;
        science->value = 10;
        speech->value = 10;
        barter->value = 10;
        survival->value = 10;

        guns->select_button->setChecked(false);
        big_guns->select_button->setChecked(false);
        unarmed->select_button->setChecked(false);
        science->select_button->setChecked(false);
        speech->select_button->setChecked(false);
        barter->select_button->setChecked(false);
        survival->select_button->setChecked(false);

        guns->update_display();
        big_guns->update_display();
        unarmed->update_display();
        science->update_display();
        speech->update_display();
        barter->update_display();
        survival->update_display();

        skill_points = 3;
        update_skill_hint();

        selected = 0;
        player_asset = "default_player.png";
        apply_asset();
    }

    void next_asset() {
        ++selected;
        if (selected >= skins.size())
            selected = 0;
        player_asset = skins[selected];
        apply_asset();
    }
    void prev_asset() {
        --selected;
        if (selected < 0)
            selected = skins.size() - 1;
        player_asset = skins[selected];
        apply_asset();
    }
public:
    void apply_asset() {
        player_asset_show->setStyleSheet(QString("border-image: url(assets:/pictures/player_skins/%1);").arg(player_asset));
    }
    void paintEvent(QPaintEvent *event) {
        QPainter paint(this);
        if (background.isNull())
            return;

        paint.drawPixmap(this->rect(), background);
    }
    QPixmap background;

    int char_points = 20;
    int skill_points = 3;

    QLabel* name_hint = nullptr;
    QLabel* char_hint = nullptr;
    QLabel* skill_hint = nullptr;

    QString player_asset = "default_player.png";
    QStringList skins;
    int selected = 0;
    unclickable_button* player_asset_show = nullptr;
    QPushButton* next_player_asset = nullptr;
    QPushButton* prev_player_asset = nullptr;

    QLineEdit* name = nullptr;

    char_selector* strength = nullptr;
    char_selector* agility = nullptr;
    char_selector* endurance = nullptr;
    char_selector* intelligence = nullptr;
    char_selector* luck = nullptr;

    skill_selector* guns = nullptr;
    skill_selector* big_guns = nullptr;
    skill_selector* unarmed = nullptr;
    skill_selector* science = nullptr;
    skill_selector* speech = nullptr;
    skill_selector* barter = nullptr;
    skill_selector* survival = nullptr;

    tracked_button* done_button = nullptr;
    tracked_button* cancel_button = nullptr;
    tracked_button* reset_button = nullptr;

    void update_char_hint() {
        try_character_ready();
        if (char_points == 0) {
            char_hint->setText(QString("Очки распределены."));
            return;
        }
        char_hint->setText(QString("Осталось очков: %1").arg(char_points));
    }
    void update_skill_hint() {
        try_character_ready();
        if (skill_points == 0) {
            skill_hint->setText(QString("Главные навыки выбраны."));
            return;
        }
        skill_hint->setText(QString("Выбрано главных навыков: %1 из 3").arg(3 - skill_points));
    }
    void try_character_ready() {
        if (this->done_button == nullptr)
            return;


        if (skill_points == 0 && char_points == 0) {
            this->done_button->setEnabled(true);
            return;
        }
        this->done_button->setEnabled(false);
    }
    void connect_char(char_selector* c) {
        this->connect(c, &char_selector::increase, this, &character_creation_widget::increase_stat);
        this->connect(c, &char_selector::decrease, this, &character_creation_widget::decrease_stat);
    }
    void connect_special_strength(char_selector* c) {
        this->connect(c, &char_selector::increase, this, &character_creation_widget::special_increase_strength);
        this->connect(c, &char_selector::decrease, this, &character_creation_widget::special_decrease_strength);
    }
    void connect_special_intelligence(char_selector* c) {
        this->connect(c, &char_selector::increase, this, &character_creation_widget::special_increase_intelligence);
        this->connect(c, &char_selector::decrease, this, &character_creation_widget::special_decrease_intelligence);
    }
    void connect_skill(skill_selector* s) {
        this->connect(s, &skill_selector::select, this, &character_creation_widget::set_primary_skill);
        this->connect(s, &skill_selector::deselect, this, &character_creation_widget::unset_primary_skill);
    }
    void modify_skill(skill_selector* s, short amount) {
        s->value += amount;
        s->update_display();
    }
    character_creation_widget(QWidget* parent = nullptr): QWidget(parent) {
        this->setGeometry(0, 0, 620, 620);
        background.load("assets:/pictures/testbkg_character_creation.jpg");

        name_hint = new QLabel("Имя:", this);
        name_hint->setGeometry(210, 530, 200, 15);

        name = new QLineEdit(this);
        name->setGeometry(210, 545, 200, 20);


        char_hint = new QLabel(this);
        char_hint->setGeometry(5, 5, 200, 15);
        update_char_hint();

        strength = new char_selector(char_type::strength, this);
        strength->move(5, 45);
        agility = new char_selector(char_type::agility, this);
        agility->move(5, 100);
        endurance = new char_selector(char_type::endurance, this);
        endurance->move(5, 155);
        intelligence = new char_selector(char_type::intelligence, this);
        intelligence->move(5, 210);
        luck = new char_selector(char_type::luck, this);
        luck->move(5, 265);
        this->connect_special_strength(strength);
        this->connect_char(agility);
        this->connect_char(endurance);
        this->connect_special_intelligence(intelligence);
        this->connect_char(luck);


        skill_hint = new QLabel(this);
        skill_hint->setGeometry(415, 5, 200, 15);
        update_skill_hint();

        guns = new skill_selector(skill_type::guns, this);
        guns->move(415, 45);
        big_guns = new skill_selector(skill_type::big_guns, this);
        big_guns->move(415, 80);
        unarmed = new skill_selector(skill_type::unarmed, this);
        unarmed->move(415, 115);
        science = new skill_selector(skill_type::science, this);
        science->move(415, 150);
        speech = new skill_selector(skill_type::speech, this);
        speech->move(415, 185);
        barter = new skill_selector(skill_type::barter, this);
        barter->move(415, 220);
        survival = new skill_selector(skill_type::survival, this);
        survival->move(415, 255);
        this->connect_skill(guns);
        this->connect_skill(big_guns);
        this->connect_skill(unarmed);
        this->connect_skill(science);
        this->connect_skill(speech);
        this->connect_skill(barter);
        this->connect_skill(survival);


        done_button = new tracked_button(this);
        done_button->setDisabled(true);
        done_button->setGeometry(5, 595, 200, 20);
        done_button->setText("Готово");
        this->connect(done_button, &tracked_button::clicked, this, &character_creation_widget::done_clicked);

        reset_button = new tracked_button(this);
        reset_button->setGeometry(210, 595, 200, 20);
        reset_button->setText("Сброс");
        this->connect(reset_button, &tracked_button::clicked, this, &character_creation_widget::reset_clicked);

        cancel_button = new tracked_button(this);
        cancel_button->setGeometry(415, 595, 200, 20);
        cancel_button->setText("Назад");
        auto current = global::w.current_scene;
        this->connect(cancel_button, &tracked_button::clicked, this, [=]() {global::w.switch_to_scene(current);});

        player_asset_show = new unclickable_button(this);
        player_asset_show->setGeometry(260, 325, 100, 200);
        player_asset_show->setStyleSheet(QString("border-image: url(assets:/pictures/player_skins/%1);").arg(player_asset));

        auto player_skins_path = global::root_path;
        player_skins_path /= "assets";
        player_skins_path /= "pictures";
        player_skins_path /= "player_skins";
        auto listing = QDirListing(QString::fromStdString(player_skins_path.string()), QDirListing::IteratorFlag::FilesOnly);
        for (const auto& elem : listing) {
            if (elem.fileName() == "default_player.png") {
                skins.insert(skins.begin(), elem.fileName());
                continue;
            }
            skins.append(elem.fileName());
        }
        if (skins.size() > 1) {
            next_player_asset = new QPushButton("След.",this);
            next_player_asset->setGeometry(370, 425, 80, 30);
            connect(next_player_asset, &QPushButton::clicked, this, &character_creation_widget::next_asset);
            prev_player_asset = new QPushButton("Пред.",this);
            prev_player_asset->setGeometry(170, 425, 80, 30);
            connect(prev_player_asset, &QPushButton::clicked, this, &character_creation_widget::prev_asset);
        }
    }
};
class skill_increaser: public skill_selector {

    Q_OBJECT
public slots:
    void try_increase() {
        emit increase(this, type, 1);
    }
signals:
    void increase(skill_increaser* obj, skill_type type_, short value);
public:
    void set_value(short value_) {
        value = value_;
        update_display();
    }
    tracked_button* increase_button = nullptr;
    QLabel* name = nullptr;
    skill_increaser(skill_type type_, QWidget* parent = nullptr): skill_selector(type_, parent) {
        this->select_button->deleteLater();
        increase_button = new tracked_button(this);
        increase_button->setGeometry(0, 0, 30, 30);
        increase_button->setStyleSheet(QString("border-image: url(assets:/pictures/char_increase.png);"));
        name = new QLabel(this);
        name->setText(skill_type_to_str(type));
        name->setGeometry(35, 0, 200, 30);
        name->setStyleSheet("color: rgb(255,255,255);");
        display->setStyleSheet(name->styleSheet());
        connect(increase_button, &tracked_button::clicked, this, &skill_increaser::try_increase);
    }

};

class character_view_widget: public QWidget, public can_play_sound {

    Q_OBJECT
public slots:
    void add_skill_point(skill_increaser* obj, skill_type type_, short value) {
        if (points <= 0) {
            new screen_message("> Отсутствуют очки навыков", 30, 60, 18);
            play_sfx(sfx_fail);
            return;
        }
        play_sfx(sfx_typing);
        --global::player_->_level_up_points;
        points = global::player_->_level_up_points;
        global::player_->_entity_stats.add_stat(type_, value);
        obj->set_value(global::player_->_entity_stats.get_stat(type_));
        update_points_hint();
    }
public:
    void paintEvent(QPaintEvent *event) {
        QPainter paint(this);
        if (background.isNull())
            return;

        paint.drawPixmap(this->rect(), background);
    }
    void update_points_hint() {
        if (points == 0) {
            points_hint->setText(QString("Очки навыков распределены."));
            return;
        }
        points_hint->setText(QString("Осталось очков навыков: %1").arg(points));
    }
    QPixmap background;

    QSoundEffect* sfx_fail = nullptr;
    QSoundEffect* sfx_typing = nullptr;

    QLabel* player_name = nullptr;
    QLabel* player_level = nullptr;

    QLabel* player_health = nullptr;
    QLabel* player_armor = nullptr;
    QLabel* player_money = nullptr;
    QLabel* player_weight = nullptr;
    QLabel* player_energy = nullptr;

    QLabel* strength = nullptr;
    QLabel* agility = nullptr;
    QLabel* endurance = nullptr;
    QLabel* intelligence = nullptr;
    QLabel* luck = nullptr;

    unclickable_button* player_show = nullptr;

    short points = 0;
    QLabel* points_hint = nullptr;

    skill_increaser* guns = nullptr;
    skill_increaser* big_guns = nullptr;
    skill_increaser* unarmed = nullptr;
    skill_increaser* science = nullptr;
    skill_increaser* speech = nullptr;
    skill_increaser* barter = nullptr;
    skill_increaser* survival = nullptr;

    void connect_increaser(skill_increaser* obj) {
        connect(obj, &skill_increaser::increase, this, &character_view_widget::add_skill_point);
        obj->set_value(global::player_->_entity_stats.get_stat(obj->type));
    }

    character_view_widget(QWidget* parent = nullptr): QWidget(parent) {

        player_show = new unclickable_button(this);
        player_show->setStyleSheet(QString("border-image: url(assets:/pictures/player_skins/%1);").arg(global::player_->_asset));
        player_show->setGeometry(712 - 152, 120, 305, 610);

        this->resize(1425, 900);
        points = global::player_->get_level_up_points();

        sfx_fail = new QSoundEffect(this);
        sfx_fail->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/fail.wav")).absoluteFilePath()));

        sfx_typing = new QSoundEffect(this);
        sfx_typing->setSource(QUrl::fromLocalFile(QFileInfo(QString("assets:sounds/typing.wav")).absoluteFilePath()));

        background.load("assets:/pictures/black.png");
        player_name = new QLabel(global::player_->get_name(), this);
        player_name->setGeometry(10,10, 1415, 70);
        player_name->setStyleSheet("color: rgb(255,255,255); font: 24pt \"Arial\";");
        player_name->setAlignment(Qt::AlignCenter);

        player_level = new QLabel(this);
        player_level->setGeometry(10, 90, 1415, 40);
        player_level->setAlignment(player_name->alignment());
        player_level->setStyleSheet("color: rgb(255,255,255); font: 16pt \"Arial\";");
        player_level->setText(QString("Уровень %1, Опыт: %2/%3").arg(global::player_->get_entity_level().level).arg(global::player_->get_entity_level().experiecne).arg(global::player_->get_entity_level().current_needed));

        player_health = new QLabel(QString("Здоровье: %1/%2").arg(global::player_->get_health()).arg(global::player_->get_max_health() + global::player_->get_bonus_health()), this);
        player_health->setGeometry(10, 150, 300, 20);
        player_health->setStyleSheet("color: rgb(255,0,0);");

        player_armor = new QLabel(QString("Броня: %1").arg(global::player_->get_inventory()->get_total_armor() + global::player_->get_base_armor() + global::player_->get_bonus_armor()), this);
        player_armor->setGeometry(10, 180, 300, 20);
        player_armor->setStyleSheet("color: rgb(90,130,180);");

        player_weight = new QLabel(QString("Вес инвентаря: %1/%2").arg(global::player_->get_weight()).arg(global::player_->get_max_weight()),this);
        player_weight->setGeometry(10, 210, 300, 20);
        player_weight->setStyleSheet("color: rgb(180,180,180);");

        player_energy = new QLabel(QString("Энергия: %1").arg(global::player_->get_max_energy() + global::player_->get_bonus_energy()),this);
        player_energy->setGeometry(10, 240, 300, 20);
        player_energy->setStyleSheet("color: rgb(80,255,80);");

        player_money = new QLabel(QString("Деньги: %1").arg(global::player_->get_money()),this);
        player_money->setGeometry(10, 270, 300, 20);
        player_money->setStyleSheet("color: rgb(255,255,80);");

        strength = new QLabel(QString("Сила: %1").arg(global::player_->get_total_stats().strength),this);
        strength->setGeometry(210, 150, 300, 20);
        strength->setStyleSheet("color: rgb(255,255,255);");

        agility = new QLabel(QString("Ловкость: %1").arg(global::player_->get_total_stats().agility),this);
        agility->setGeometry(210, 180, 300, 20);
        agility->setStyleSheet("color: rgb(255,255,255);");

        endurance = new QLabel(QString("Стойкость: %1").arg(global::player_->get_total_stats().endurance),this);
        endurance->setGeometry(210, 210, 300, 20);
        endurance->setStyleSheet("color: rgb(255,255,255);");

        intelligence = new QLabel(QString("Интеллект: %1").arg(global::player_->get_total_stats().intelligence),this);
        intelligence->setGeometry(210, 240, 300, 20);
        intelligence->setStyleSheet("color: rgb(255,255,255);");

        luck = new QLabel(QString("Удача: %1").arg(global::player_->get_total_stats().luck),this);
        luck->setGeometry(210, 270, 300, 20);
        luck->setStyleSheet("color: rgb(255,255,255);");

        points_hint = new QLabel(this);
        points_hint->setGeometry(10, 350, 400, 20);
        points_hint->setStyleSheet("color: rgb(255,255,255);");
        update_points_hint();

        guns = new skill_increaser(skill_type::guns, this);
        connect_increaser(guns);
        guns->move(10, 380);
        big_guns = new skill_increaser(skill_type::big_guns, this);
        connect_increaser(big_guns);
        big_guns->move(10, 415);
        unarmed = new skill_increaser(skill_type::unarmed, this);
        connect_increaser(unarmed);
        unarmed->move(10, 450);
        science = new skill_increaser(skill_type::science, this);
        connect_increaser(science);
        science->move(10, 485);
        speech = new skill_increaser(skill_type::speech, this);
        connect_increaser(speech);
        speech->move(10, 520);
        barter = new skill_increaser(skill_type::barter, this);
        connect_increaser(barter);
        barter->move(10, 555);
        survival = new skill_increaser(skill_type::survival, this);
        connect_increaser(survival);
        survival->move(10, 590);
    }

};
