/*
 * File:   openglwidget.h
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QGLWidget>
#include <QEvent>
#include "opengldelegate.h"

class QSize;
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;
class QDropEvent;
class QDragEnterEvent;
class QThread;
class QWaitCondition;
class QMutex;

class OpenGLWidget : public QGLWidget, public OpenGLDelegate
{
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLWidget)
public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

protected:
    virtual bool event(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

public slots:
    virtual void moveContextToDeviceThread();
    virtual void moveContextToMainThread();
    virtual void fillWithColor(const QColor &color = Qt::black);
    virtual void swapBuffer();
    virtual void makeContextCurrent();
    virtual void getWindowSize(int *width, int *height);

public:
    static const QEvent::Type m_moveContextEvent;

private:
    QMutex *m_contextMovedLock;
    QWaitCondition *m_contextMoved;
    bool m_contextInMainThread;
};

class MoveContextEvent : public QEvent
{
public:
    MoveContextEvent(QThread *destThread) :
        QEvent(OpenGLWidget::m_moveContextEvent),
        m_destThread(destThread)
    {
    }
    QThread* getThread() const { return m_destThread; }
private:
    QThread *m_destThread;
};


#endif