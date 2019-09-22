#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QVlcPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_openFileAction_triggered();
    void on_exitAction_triggered();
    void on_playPauseButton_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_stopButton_clicked();
    void on_positionSlider_sliderMoved(int position);
    void on_volumeSlider_sliderMoved(int position);
    void on_playlistWidget_clicked(const QModelIndex &index);
private:
    Ui::MainWindow *ui;
    QVlcPlayer *mMediaPlayer;
    QVlcPlayer *makeMediaPlayer();

    void addMedia(const QString &media);
    void loadPlaylistFromDir(QString dirMediaPath);
};
#endif // MAINWINDOW_H
