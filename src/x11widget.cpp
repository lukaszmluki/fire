/*
 * File:   x11widget.cpp
 * Author: Lukasz Marek
 *
 * Created on April 17, 2013, 10:08 PM
 */

#include "x11widget.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include "playermanager.h"
#include "ffengine.h"

X11Widget::X11Widget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(false);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_PaintOnScreen);
}

X11Widget::~X11Widget()
{
}

void X11Widget::swapBuffer()
{
}

void X11Widget::prepareBuffer()
{
}

int X11Widget::windowWidth()
{
    return QWidget::width();
}

int X11Widget::windowHeight()
{
    return QWidget::height();
}

void X11Widget::fillWithColor(const QColor &color)
{
    QPainter painter(this);
    painter.fillRect(QRect(QPoint(0, 0), size()), color);
}

bool X11Widget::event(QEvent *event)
{
    return QWidget::event(event);
}

void X11Widget::resizeEvent(QResizeEvent *event)
{
    event->accept();
}

void X11Widget::paintEvent(QPaintEvent *event)
{
    if (!PlayerManager::instance().getPlayer(this)->isMediaOpened())
        fillWithColor();
    else
        PlayerManager::instance().getPlayer(this)->repaint(event->rect());
    event->accept();
}

QString X11Widget::engineName()
{
    return QString("X11");
}

