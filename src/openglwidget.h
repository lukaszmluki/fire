/*
 * File:   openglwidget.h
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <QGLWidget>
#include "opengldelegate.h"

class QSize;
class QResizeEvent;
class QPaintEvent;
class QMouseEvent;
class QDropEvent;
class QDragEnterEvent;

class OpenGLWidget : public QGLWidget, public OpenGLDelegate
{
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLWidget)
public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

protected:
    virtual void paintGL();

public slots:
    virtual void fillWithColor(const QColor &color = Qt::black);
    virtual void swapBuffer();
    virtual void makeContextCurrent();
    virtual void getWindowSize(int *width, int *height);
};

#endif
