#include "qvlcplayer.h"
#include <QtConcurrent>
#include <QWidget>
#include <QTimer>

QVlcPlayer::QVlcPlayer(QObject *parent)
    : QVlcCore { parent }
{
}

void QVlcPlayer::play()
{
    if (!m_media_location.isEmpty())
    {
        if (m_media_state == libvlc_Paused)
        {
            libvlc_media_player_pause(m_media_player);
        }
        else if (m_media_state == libvlc_Stopped)
        {
            m_timer->start();
            libvlc_media_player_play(m_media_player);
        }
        else
        {
            libvlc_media_player_play(m_media_player);
        }
    }
}

void QVlcPlayer::pause()
{
    if (m_media_state != libvlc_Paused)
    {
        libvlc_media_player_pause(m_media_player);
    }
}

void QVlcPlayer::stop()
{
    if (m_media_state != libvlc_Stopped && m_media_state != libvlc_NothingSpecial)
    {
        m_timer->stop();
        m_position = -1.0f;
        m_media_state = libvlc_Stopped;
        libvlc_media_release(m_media);
        libvlc_media_player_release(m_media_player);
        libvlc_release(m_instance);
        m_instance = libvlc_new(0, nullptr);
        m_media_player = libvlc_media_player_new(m_instance);
        m_media = nullptr;
        if (m_video_widget)
        {
            libvlc_media_player_set_hwnd(m_media_player, reinterpret_cast<void *>(m_video_widget->winId()));
        }
        setMedia(m_playlist.at(m_index));
    }
}

void QVlcPlayer::next()
{
    playAt(getNextIdx(m_index));
}

void QVlcPlayer::prev()
{
    playAt(getPrevIdx(m_index));
}

void QVlcPlayer::playAt(int index)
{
    setIndex(index);
    setMedia(m_playlist.at(m_index));
    QtConcurrent::run([&]()
    {
        play();
    });
}

void QVlcPlayer::addMedia(const QString &media)
{
    m_playlist.append(media);
}

void QVlcPlayer::setVideoWidget(QWidget *videoWidget)
{
    m_video_widget = videoWidget;
    libvlc_media_player_set_hwnd(m_media_player, reinterpret_cast<void *>(m_video_widget->winId()));
}

void QVlcPlayer::setVolume(int volume)
{
    libvlc_audio_set_volume(m_media_player, volume);
}

void QVlcPlayer::setPosition(int position)
{
    libvlc_media_player_set_position(m_media_player, position / 100.0f);
}

int QVlcPlayer::currentPosition() const
{
    return int(m_position * 100.0f);
}

int QVlcPlayer::currentIndex() const
{
    return m_index;
}

libvlc_state_t QVlcPlayer::currentState() const
{
    return m_media_state;
}

libvlc_time_t QVlcPlayer::currentDuration() const
{
    return m_duration;
}
