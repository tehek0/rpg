#pragma once
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTemporaryFile>
#include <iostream>

class music_manager {
protected:
    QMediaPlayer* _player;
    QAudioOutput* _output;

public:
    music_manager() {
        _player = new QMediaPlayer();
        _output = new QAudioOutput();
        _player->setAudioOutput(_output);
    }
    void change_volume(float volume) {
        _output->setVolume(volume);
    }
    void stop() {
        _player->stop();
    }
    void set_music(const QString& string) {
        if (_player->isPlaying())
            stop();

        _player->setSource(QUrl(QString("qrc:/sounds/music/%1.ogg").arg(string)));
        _player->setLoops(QMediaPlayer::Loops::Infinite);
        _player->play();
    }
    QMediaPlayer* get_player() {
        return _player;
    }
};
