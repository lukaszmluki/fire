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
#include "videowidget.h"

class QSize;
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;
class QDropEvent;
class QDragEnterEvent;
class QThread;
class QWaitCondition;
class QMutex;

class OpenGLWidget : public QGLWidget, public VideoWidget
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
    virtual void swapBuffer();
    virtual void prepareBuffer();
    virtual int windowWidth();
    virtual int windowHeight();

private:
    void fillWithColor(const QColor &color = Qt::black);
};

#endif
