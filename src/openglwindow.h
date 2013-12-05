/*
 * File:   openglwindow.h
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#ifndef SRC_OPENGL_WINDOW_H
#define SRC_OPENGL_WINDOW_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    virtual ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderNow();

protected:
    bool event(QEvent *event);

    void exposeEvent(QExposeEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    bool m_update_pending;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
};

#endif /* SRC_OPENGL_WINDOW_H */