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

const QEvent::Type OpenGLWidget::m_moveContextEvent = static_cast<QEvent::Type>(QEvent::registerEventType());

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer | QGL::NoDepthBuffer), parent),
    m_contextMovedLock(new QMutex),
    m_contextMoved(new QWaitCondition),
    m_contextInMainThread(true)
{
    setAutoFillBackground(false);
    QGLWidget::makeCurrent();
    QGLWidget::doneCurrent();
}

OpenGLWidget::~OpenGLWidget()
{
    m_contextMovedLock->lock();
    if (!m_contextInMainThread)
        m_contextMoved->wait(m_contextMovedLock);
    m_contextMovedLock->unlock();
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
    m_contextMovedLock->lock();
    QCoreApplication::postEvent(this, new MoveContextEvent(QThread::currentThread()));
    m_contextMoved->wait(m_contextMovedLock);
    m_contextMovedLock->unlock();
}

void OpenGLWidget::moveContextToMainThread()
{
    m_contextMovedLock->lock();
    context()->moveToThread(thread());
    m_contextInMainThread = true;
    m_contextMovedLock->unlock();
    m_contextMoved->wakeAll();
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
    if (event->type() == m_moveContextEvent) {
        MoveContextEvent *e = static_cast<MoveContextEvent *>(event);
        m_contextMovedLock->lock();
        context()->moveToThread(e->getThread());
        m_contextInMainThread = false;
        m_contextMovedLock->unlock();
        m_contextMoved->wakeAll();
        return true;
    }
    return QGLWidget::event(event);
}

void OpenGLWidget::resizeEvent(QResizeEvent *event)
{
    event->accept();
}

void OpenGLWidget::paintEvent(QPaintEvent *event)
{
    if (m_contextInMainThread)
        fillWithColor();
    event->accept();
}

