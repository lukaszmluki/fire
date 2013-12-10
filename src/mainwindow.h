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

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MainWindow)
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

public slots:
//    void quit();
    void play();
    void stop();
    void pause();
//    void next();
//    void previous();
//    void forward();
//    void backward();
    void loadVideo();
    void closeVideo();
    void showEditor();
    void hideEditor();

protected:
    void resizeEvent(QResizeEvent *e);
    void moveEvent(QMoveEvent *e);

private:
    static QAction* addMenuAction(QMenu *menu, const QString &iconFile, const QString &text, const QObject *receiver, const char *member, const QKeySequence &shortcut, const QVariant &data, QActionGroup *group = NULL, bool showIcon = true);
    void addNavigationButton(const QString &iconFile, const char* member, QHBoxLayout *layout);

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
};

#endif	/* SRC_MAINWINDOW_H */
