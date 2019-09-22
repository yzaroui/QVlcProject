#ifndef QVLCPLAYER_H
#define QVLCPLAYER_H

#include "qvlccore.h"

class QVlcPlayer : public QVlcCore
{
    Q_OBJECT
public:
    QVlcPlayer(QObject *parent = nullptr);
    void play();
    void pause();
    void stop();
    void next();
    void prev();
    void playAt(int index);
    void addMedia(const QString &media);
    void setVideoWidget(QWidget *videoWidget);
    void setVolume(int volume);
    void setPosition(int position);
    int currentPosition() const;
    int currentIndex() const;
    libvlc_state_t currentState() const;
    libvlc_time_t currentDuration() const;
};

#endif // QVLCPLAYER_H
