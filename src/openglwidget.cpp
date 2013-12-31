/*
 * File:   openglwidget.cpp
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#include "openglwidget.h"
#include <QDebug>
#include <QThread>
#include <QCoreApplication>
#include <QWaitCondition>
#include <QMutex>
#include <QPaintEvent>
#include "playermanager.h"
#include "ffengine.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer | QGL::NoDepthBuffer), parent)
{
    setAutoFillBackground(false);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::swapBuffer()
{
    QGLWidget::swapBuffers();
    QGLWidget::doneCurrent();
}

void OpenGLWidget::makeContextCurrent()
{
    QGLWidget::makeCurrent();
}

void OpenGLWidget::moveContextToDeviceThread()
{
}

void OpenGLWidget::moveContextToMainThread()
{
}

void OpenGLWidget::getWindowSize(int *width, int *height)
{
    if (width)
        *width = QGLWidget::width();
    if (height)
        *height = QGLWidget::height();
}

void OpenGLWidget::fillWithColor(const QColor &color)
{
    makeCurrent();
    qglClearColor(color);
    glClear(GL_COLOR_BUFFER_BIT);
    swapBuffer();
}

bool OpenGLWidget::event(QEvent *event)
{
    return QGLWidget::event(event);
}

void OpenGLWidget::resizeEvent(QResizeEvent *event)
{
    event->accept();
}

void OpenGLWidget::paintEvent(QPaintEvent *event)
{
    if (PlayerManager::instance().getPlayer(this)->isMediaOpened())
        ;//repaint
    else
        fillWithColor();
    event->accept();
}

