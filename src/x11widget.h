/*
 * File:   x11widget.h
 * Author: Lukasz Marek
 *
 * Created on April 17, 2013, 10:08 PM
 */

#ifndef SRC_X11_WIDGET_H
#define SRC_X11_WIDGET_H

#include <QWidget>
#include <QEvent>
#include "videowidget.h"

class QResizeEvent;
class QPaintEvent;

class X11Widget : public QWidget, public VideoWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(X11Widget)
public:
    X11Widget(QWidget *parent = 0);
    ~X11Widget();

protected:
    bool event(QEvent *event);
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

#endif /* SRC_X11_WIDGET_H */
