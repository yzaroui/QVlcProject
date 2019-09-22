#include "qvlc.h"
#include <QTimer>

QVlc::QVlc(QObject *parent) : QObject { parent }
{
    m_instance = libvlc_new(0, nullptr);
    m_media_player = libvlc_media_player_new(m_instance);
    m_media = nullptr;
    m_media_state = libvlc_NothingSpecial;
    m_duration = -1;
    m_position = -1.0f;
    m_index = -1;
    m_playlist.clear();
    m_media_location = "";
    m_video_widget = nullptr;
    m_timer = makeTimer();
    m_timer->start();
}

QVlc::~QVlc()
{
    m_timer->stop();
    m_video_widget = nullptr;
    m_media_location = "";
    m_playlist.clear();
    m_index = -1;
    m_position = -1.0f;
    m_duration = -1;
    m_media_state = libvlc_NothingSpecial;
    libvlc_media_release(m_media);
    libvlc_media_player_release(m_media_player);
    libvlc_release(m_instance);
    m_media = nullptr;
    m_media_player = nullptr;
    m_instance = nullptr;
}

QTimer *QVlc::makeTimer()
{
    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [&]()
    {
        if (m_media_player)
        {
            setPosition(libvlc_media_player_get_position(m_media_player));
            setState(libvlc_media_player_get_state(m_media_player));
        }
        if (m_media)
        {
            setDuration(libvlc_media_get_duration(m_media));
        }
    });

    return timer;
}
