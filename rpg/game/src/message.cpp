#include "../header/message.h"
#include "../header/global.h"

void screen_message::next_frame() {
    ++ticks;
    if (ticks > ticks_to_go) {
        this->deleteLater();
        return;
    }
    float opacity_coef = smoothstep_algorythm(ticks, ticks_to_go);
    opacity->setOpacity(opacity_coef);
}
screen_message::screen_message(const QString& message, unsigned int last_for, int stay_still, unsigned int font_size, int color_r, int color_g, int color_b): QLabel(&global::w) {
    ticks = 0 - stay_still;
    this->setGeometry(0,0,global::window_width, global::window_height);
    this->setAlignment(Qt::AlignTop | Qt::AlignRight);
    this->setStyleSheet(QString("font: %1pt \"Arial\"; color: rgb(%2,%3,%4);").arg(font_size).arg(color_r).arg(color_g).arg(color_b));
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setText(message);
    this->setContentsMargins(QMargins(8, 8, 8, 8));
    ticks_to_go = last_for;
    opacity = new QGraphicsOpacityEffect(this);
    opacity->setOpacity(1);
    this->setGraphicsEffect(opacity);
    connect(global::timer, &QTimer::timeout, this, &screen_message::next_frame);
    this->show();
    this->adjustSize();
    this->resize(global::window_width, this->height());
    auto prev_message = global::w.findChild<screen_message*>("last_screen_message", Qt::FindDirectChildrenOnly);
    if (prev_message != nullptr) {
        prev_message->setObjectName("not_last_screen_message");
        if (!(prev_message->pos().y() + this->height() > global::window_height))
            this->move(prev_message->pos().x(), prev_message->pos().y() + this->height());
    }
    this->setObjectName("last_screen_message");
}
float screen_message::smoothstep_algorythm(float steps, float required_steps) {
    float process = steps / required_steps;
    if (process > 1.0f) {
        process = 1.0f;
    }
    else if (process < 0.0f) {
        process = 0.0f;
    }
    return 1 - (process * process * (3.0f - 2.0f * process));
}
