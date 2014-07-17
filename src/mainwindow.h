/*
 * File:   mainwindow.h
 * Author: Lukasz Marek
 *
 * Created on December 10, 2013, 10:08 PM
 */

#ifndef SRC_MAINWINDOW_H
#define	SRC_MAINWINDOW_H

#include <QMainWindow>
#include "guidelegate.h"
#include "videowidget.h"

class OpenGLWidget;
class SubtitlesEditor;

class QResizeEvent;
class QMoveEvent;
class QWidget;
class QLabel;
class QAction;
class QActionGroup;
class QHBoxLayout;
class QToolBar;
class QSlider;
class QSplitter;
class QToolButton;

class MainWindow : public QMainWindow, public GuiDelegate
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
//    void quit();
    void togglePause();
    void stop();
//    void next();
//    void previous();
//    void forward();
//    void backward();
    void loadVideo();
    void closeVideo();
    void showEditor();
    void hideEditor();
    void toggleEditor();
    void updateEditorVisibility();
    void showPlaylist();
    void hidePlaylist();
    void togglePlaylist();
    void updatePlaylistVisibility();

private slots:
    /* gui delegate */
    void durationChanged(double duration);
    void positionChanged(double position);
    void muteChanged(int mute);
    void volumeChanged(double volume);
    void paused();
    void resumed();
    /* private */
    void positionSliderChanged(int position);
    void volumeSliderChanged(int volume);
    void muteButtonClicked();
    void saveSplitterState(int pos, int index);

protected:
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);

private:
    //GUI construction
    QAction* addMenuAction(QMenu *menu, const QString &iconFile, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut, const QVariant &data, QActionGroup *group = NULL, bool showIcon = true);
    QToolButton* addNavigationButton(const QString &iconFile, const char* member, QHBoxLayout *layout);

    void createCentralWidget();
    void createMovieLine();
    void createVideoArea();
    void createVideoNavigator();
    void createSubtitlesEditorArea();
    void createPlaylistArea();
    void createMenu();

    //GUI elements
    QWidget *m_centralWidget;

    QWidget *m_movieLine;
    QLabel  *m_moviePosition;
    QLabel  *m_movieInfo;

    QWidget *m_videoArea;
    VideoWidget *m_videoWidget;
    QWidget *m_navigationPanel;
    QSlider *m_positionSlider;
    QToolButton *m_playButton;
    QSlider *m_volumeSlider;

    QSplitter *m_splitterVideoSubtitlesArea;
    QSplitter *m_splitterVideoPlaylistArea;

    QWidget *m_subtitlesEditorArea;
    SubtitlesEditor *m_subtitlesEditor;
    QLabel *m_editorFileName;
    QLabel *m_editorInfoLine;

    QWidget *m_playlistArea;

    QMenu *m_videoStreamsSubMenu;
    QMenu *m_audioStreamsSubMenu;
    QActionGroup *m_videoStreamsGroup;
    QActionGroup *m_audioStreamsGroup;
    //QToolBar *m_fileToolBar;
};

#endif	/* SRC_MAINWINDOW_H */
