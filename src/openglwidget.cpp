/*
 * File:   openglwidget.cpp
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:08 PM
 */

#include "openglwidget.h"
#include <QThread>
#include <QCoreApplication>
#include <QWaitCondition>
#include <QMutex>
#include <QPaintEvent>

const QEvent::Type OpenGLWidget::m_moveContextEvent = static_cast<QEvent::Type>(QEvent::registerEventType());

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer | QGL::NoDepthBuffer), parent),
    m_contextMovedLock(new QMutex),
    m_contextMoved(new QWaitCondition)
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
    QCoreApplication::postEvent(this, new MoveContextEvent(NULL));
    m_contextMoved->wait(m_contextMovedLock);
    m_contextMovedLock->unlock();
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
        QThread *thread = e->getThread();
        if (!thread)
            thread = QThread::currentThread();
        m_contextMovedLock->lock();
        context()->moveToThread(thread);
        m_contextMovedLock->unlock();
        m_contextMoved->wakeAll();
        return true;
    }
    return QGLWidget::event(event);
}

void OpenGLWidget::resizeGL(int width, int height)
{
    Q_UNUSED(width)
    Q_UNUSED(height)
}

void OpenGLWidget::paintGL()
{
    //fillWithColor();
}

void OpenGLWidget::paintEvent(QPaintEvent *event)
{
    event->accept();
}
