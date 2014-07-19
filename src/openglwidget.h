/*
 * File:   openglwidget.h
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#ifndef SRC_OPENGL_WIDGET_H
#define SRC_OPENGL_WIDGET_H

#include <QGLWidget>
#include <QEvent>
#include "videowidget.h"

class QResizeEvent;
class QPaintEvent;

class OpenGLWidget : public QGLWidget, public VideoWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(OpenGLWidget)
public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void swapBuffer();
    void prepareBuffer();

public:
    int windowWidth();
    int windowHeight();
    QString engineName();

private:
    void fillWithColor(const QColor &color = Qt::black);
};

#endif /* SRC_OPENGL_WIDGET_H */
