/*
 * File:   openglwidget.cpp
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#include "openglwidget.h"
#include <QDebug>
#include <QThread>

OpenGLWidget::OpenGLWidget(QWidget *parent) :
//    QGLWidget(QGLFormat(QGL::SampleBuffers| QGL::DoubleBuffer | QGL::NoDepthBuffer | QGL::AlphaChannel | QGL::Rgba), parent)
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setAutoFillBackground(false);
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::swapBuffer()
{
    QGLWidget::swapBuffers();
}

void OpenGLWidget::makeContextCurrent()
{
    qDebug() << "thread" << QThread::currentThread();
    context()->moveToThread(QThread::currentThread());
    QGLWidget::makeCurrent();
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

void OpenGLWidget::paintGL()
{
    fillWithColor(Qt::red);
}