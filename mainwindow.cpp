#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qvlcplayer.h"
#include <QDebug>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mMediaPlayer = makeMediaPlayer();

    QString dirMediaPath = "C:/users/cedo/desktop/trabajando/reproductorperuano/lagratis";
    loadPlaylistFromDir(dirMediaPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openFileAction_triggered()
{
    QStringList archivos = QFileDialog::getOpenFileNames(this, "Abrir archivos", QDir::rootPath());
    const int nArchivos = archivos.size();
    for (int iArchivo = 0; iArchivo < nArchivos; ++iArchivo)
    {
        addMedia(archivos.at(iArchivo));
    }
}

void MainWindow::on_exitAction_triggered()
{
    close();
}

void MainWindow::on_playPauseButton_clicked()
{
    if (mMediaPlayer->currentState() == libvlc_Playing)
    {
        mMediaPlayer->pause();
    }
    else
    {
        mMediaPlayer->play();
    }
}

void MainWindow::on_prevButton_clicked()
{
    mMediaPlayer->prev();
}

void MainWindow::on_nextButton_clicked()
{
    mMediaPlayer->next();
}

void MainWindow::on_stopButton_clicked()
{
    mMediaPlayer->stop();
}

void MainWindow::on_positionSlider_sliderMoved(int position)
{
    mMediaPlayer->setPosition(position);
}

void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    mMediaPlayer->setVolume(position);
}

void MainWindow::on_playlistWidget_clicked(const QModelIndex &index)
{
    mMediaPlayer->playAt(index.row());
}

void MainWindow::addMedia(const QString &media)
{
    ui->playlistWidget->addItem(media);
    mMediaPlayer->addMedia(media);
}

QVlcPlayer *MainWindow::makeMediaPlayer()
{
    QVlcPlayer *mediaPlayer = new QVlcPlayer(this);
    mediaPlayer->setVideoWidget(ui->videoWidget);
    mediaPlayer->setVolume(ui->volumeSlider->value());

    connect(mediaPlayer, &QVlcCore::positionChanged, this, [&](int position)
    {
        ui->positionSlider->setValue(position);
    });

    connect(mediaPlayer, &QVlcCore::indexChanged, this, [&](int index)
    {
        ui->playlistWidget->setCurrentRow(index);
    });

    return mediaPlayer;
}

void MainWindow::loadPlaylistFromDir(QString dirMediaPath)
{
    QDir dirMedia(dirMediaPath);
    dirMedia.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    const int nFiles = int(dirMedia.count());
    for (int iFile = 0; iFile < nFiles; ++iFile)
    {
        QString media = dirMediaPath + "/" + dirMedia[iFile];
        addMedia(media);
    }
}
