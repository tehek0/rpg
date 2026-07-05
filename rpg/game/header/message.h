#pragma once
#include <QLabel>
#include <QGraphicsEffect>

class screen_message: public QLabel {

    Q_OBJECT

public slots:
    void next_frame();
public:
    int ticks = 0;
    int ticks_to_go;
    QGraphicsOpacityEffect* opacity;
    screen_message(const QString& message, unsigned int last_for, int stay_still, unsigned int font_size, int color_r = 255, int color_g = 255, int color_b = 255);
    float smoothstep_algorythm(float steps, float required_steps);
};
