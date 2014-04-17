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

void OpenGLWidget::prepareBuffer()
{
    QGLWidget::makeCurrent();
}

int OpenGLWidget::windowWidth()
{
    return QGLWidget::width();
}

int OpenGLWidget::windowHeight()
{
    return QGLWidget::height();
}

void OpenGLWidget::fillWithColor(const QColor &color)
{
    QGLWidget::makeCurrent();
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
    PlayerManager::instance().getPlayer(this)->resize(event->size());
    event->accept();
}

void OpenGLWidget::paintEvent(QPaintEvent *event)
{
    if (!PlayerManager::instance().getPlayer(this)->isMediaOpened())
        fillWithColor();
    event->accept();
}

QString OpenGLWidget::engineName()
{
    return QString("OpenGL");
}
