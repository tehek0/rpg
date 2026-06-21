#pragma once
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
#include "global.h"
#include "character.h"
#include "data/tooltip_types.h"
#include "data/inventory_contexts.h"

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
    tracked_button(): QPushButton()
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
        _disp = new tracked_button();
        _disp->setStyleSheet(QString("border-image: url(:/pictures/%1.png);").arg(_sprite_family));
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
    void set_sprite_family(QString& sprite_family);
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
    virtual void catch_disp_delete() {
        _disp = nullptr;
        this->disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::next_frame);
        this->disconnect(global::timer, &QTimer::timeout, this, &animated_displayable::next_step);
        this->deleteLater();
    }
public:
    animated_displayable() = default;
    animated_displayable(QString sprite_family, const anim_sequence& anim_sequence_ = anim_sequence(), const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool clickable = true): displayable(sprite_family, coord, size, clickable) {
        _anim_sequence = anim_sequence_;
        _disp->setStyleSheet(QString("border-image: url(:/pictures/animated/%1/base_sprite.png);").arg(_sprite_family));
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

        _disp->linked_tooltip->setText(linked_item->get_tooltip_text());
    }
    virtual void clicked() {
        emit click_send_to_parent();
    }
signals:
    void click_send_to_parent(); // нужно запрашивать контекст инвентаря у списка предметов, чтобы вызывать правильный попап
public:
    item* linked_item;
    QLabel* stack_label;
    bool is_dummy;
    item_object() {
        is_dummy = true;
    } // это обязательно
    item_object(item* linked_item_, const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100), bool clickable = true): displayable(linked_item_->get_asset(), coord, size, clickable), linked_item(linked_item_)
    {
        is_dummy = false;
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
        if (linked_item->get_stack() > 1) {
            stack_label->setText(QString("x%1").arg(linked_item->get_stack()));
            return;
        }

        stack_label->clear();
    };
};

class inventory_background : public QWidget {

    Q_OBJECT

signals:
    void scrolled(int delta);

public:
    QPixmap sprite;
    inventory_background(const QString& asset, QWidget* parent = nullptr): QWidget(parent) {
        sprite.load(QString(":/pictures/%1.png").arg(asset));
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

class inventory_object : public QObject {

    Q_OBJECT

public slots:
    void process_item_click(item_object* item_obj) {
        linked_inventory->remove_item(linked_inventory->get_slot(item_obj->linked_item), 1);
        // linked_inventory->add_item(new item("Имя","Фамилия", "icon_inv_armor_pot", 1, 10, 1.0f, 1, true));
    }
    void update(unsigned int slot, inv_update_context context_) {
        unsigned int lower_boundry = _scrolled_cols * _cols;
        unsigned int upper_boundry = lower_boundry + (_cols * _displayed_rows) - 1;
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
    inventory_object(inventory* link_inventory, inventory_context inventory_context_ = inventory_context::container_self, unsigned short columns = 3, unsigned short rows = 10, unsigned int item_size = 150, const QPoint& coord = QPoint(0,0)) {
        _cols = columns;
        _displayed_rows = rows;
        _item_size = item_size;
        context = inventory_context_;
        linked_inventory = link_inventory;
        base = new inventory_background("inventory_background_tile",&global::w);
        base->setGeometry(QRect(coord, QSize(_cols*_item_size, _displayed_rows*_item_size)));
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


class entity_object: public animated_displayable {

    Q_OBJECT

public slots:
    void markdown_entity() {
        if (_disp->linked_tooltip == nullptr)
            return;


        _disp->linked_tooltip->setText(QString("<center><font size=\"4\">%1</font></center>").arg(linked_entity->get_name()));
    }
public:
    entity* linked_entity;
    entity_object(entity* linked_entity_, const anim_sequence& anim_sequence_ = anim_sequence(), const QPoint& coord = QPoint(0, 0), const QSize& size = QSize(100, 100))
        : linked_entity(linked_entity_), animated_displayable(linked_entity_->get_asset(), anim_sequence_, coord, size)
    {
        _disp->tooltip = tooltip_types::name_display;
        connect(_disp, &tracked_button::request_tooltip, this, &entity_object::markdown_entity);
    }

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
        _processed_string = text_source + QString(" <img src=\":/pictures/ui_text_go_next.png\" width=\"15\" height=\"15\" style=\"vertical-align: middle;\">");
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
            _sfx->setSource(QUrl(QString("qrc:/sounds/%1.wav").arg(symbol_sound)));
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

class text_holder: public QPushButton {
    Q_OBJECT
protected:
    text_object* _linked_text_object = nullptr;
public:
    QPixmap sprite;
    text_holder(const QString& asset, QSize size = QSize(1920,300), QPoint point = QPoint(0,0), QWidget* parent = &global::w): QPushButton(parent) {
        sprite.load(QString(":/pictures/%1.png").arg(asset));
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
