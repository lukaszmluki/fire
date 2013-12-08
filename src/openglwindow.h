/*
 * File:   openglwindow.h
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#ifndef SRC_OPENGL_WINDOW_H
#define SRC_OPENGL_WINDOW_H

#include <QWindow>

class FFEngine;
class QOpenGLContext;
class QTimer;

class OpenGLWindow : public QWindow
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    virtual ~OpenGLWindow();

public slots:
    void toggleFullscreen();
    //for OpenGL
    void swapBuffer();
    void makeContextCurrent();
    void getWindowSize(int *width, int *height);

private slots:
    void openSelectedFile();
    void hideCursor();
    void showCursor();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent * event);

private:
    QOpenGLContext *m_context;
    FFEngine *m_engine;
    QTimer *m_cursorHideTimer;
};

#endif /* SRC_OPENGL_WINDOW_H */