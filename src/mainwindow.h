/*
 * File:   mainwindow.h
 * Author: Lukasz Marek
 *
 * Created on December 10, 2013, 10:08 PM
 */

#ifndef SRC_MAINWINDOW_H
#define	SRC_MAINWINDOW_H

#include <QMainWindow>
#include "opengldelegate.h"
#include "guidelegate.h"

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
    void muteChanged(int mute);
    void volumeChanged(double volume);

private slots:
    void paused();
    void resumed();
    void durationChanged(double duration);
    void positionChanged(double position);
    void positionSliderChanged(int position);
    void volumeSliderChanged(int volume);
    void muteButtonClicked();

protected:
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);

private:
    QAction* addMenuAction(QMenu *menu, const QString &iconFile, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut, const QVariant &data, QActionGroup *group = NULL, bool showIcon = true);
    QToolButton* addNavigationButton(const QString &iconFile, const char* member, QHBoxLayout *layout);

    QWidget *m_normalViewArea;
    QWidget *m_subtitlesEditorBox;
    QLabel *m_videoLinePosition;
    QLabel *m_videoLineInfo;
    QLabel *m_editorFileName;
    QLabel *m_editorInfoLine;
    QWidget *m_movieInfoLine;
    QWidget *m_navigationPanel;
    QMenu *m_videoStreamsSubMenu;
    QMenu *m_audioStreamsSubMenu;
    QActionGroup *m_videoStreamsGroup;
    QActionGroup *m_audioStreamsGroup;
    QToolBar *m_fileToolBar;
    QSlider *m_positionSlider;
    OpenGLWidget *m_videoArea;
    SubtitlesEditor *m_subtitlesEditor;
    QSplitter *m_splitterVideoEditor;
    QToolButton *m_playButton;
    QSlider *m_volumeSlider;
};

#endif	/* SRC_MAINWINDOW_H */
