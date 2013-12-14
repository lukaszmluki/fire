/*
 * File:   openglwindow.h
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#ifndef SRC_OPENGL_WINDOW_H
#define SRC_OPENGL_WINDOW_H

#include <QWindow>
#include "opengldelegate.h"

class FFEngine;
class QOpenGLContext;
class QTimer;

class OpenGLWindow : public QWindow, public OpenGLDelegate
{
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLWindow)
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    virtual ~OpenGLWindow();

public slots:
    void toggleFullscreen();
    //for OpenGL
    virtual void fillWithColor(const QColor &color = Qt::black);
    virtual void swapBuffer();
    virtual void makeContextCurrent();
    virtual void getWindowSize(int *width, int *height);
    virtual void moveContextToDeviceThread() {}
    virtual void moveContextToMainThread() {}

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
    QTimer *m_cursorHideTimer;
};

#endif /* SRC_OPENGL_WINDOW_H */