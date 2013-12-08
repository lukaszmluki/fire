/*
 * File:   openglwindow.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#include "openglwindow.h"
#include <QTimer>
#include <QCursor>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QCoreApplication>
#include "ffengine.h"
#include "preferences.h"

OpenGLWindow::OpenGLWindow(QWindow *parent) :
    QWindow(parent),
    m_context(0),
    m_engine(new FFEngine("opengl", "pulse", this)),
    m_cursorHideTimer(new QTimer(this))
{
    setSurfaceType(QWindow::OpenGLSurface);
    
    QObject::connect(m_engine, SIGNAL(beforeWriteHeader()), this, SLOT(makeContextCurrent()), Qt::DirectConnection);
    QObject::connect(m_engine, SIGNAL(beforeWritePacket()), this, SLOT(makeContextCurrent()), Qt::DirectConnection);
    QObject::connect(m_engine, SIGNAL(beforeWriteTrailer()), this, SLOT(makeContextCurrent()), Qt::DirectConnection);

    QObject::connect(m_engine, SIGNAL(afterWriteHeader()), this, SLOT(swapBuffer()), Qt::DirectConnection);
    QObject::connect(m_engine, SIGNAL(afterWritePacket()), this, SLOT(swapBuffer()), Qt::DirectConnection);
    QObject::connect(m_engine, SIGNAL(afterWriteTrailer()), this, SLOT(swapBuffer()), Qt::DirectConnection);
    
    QObject::connect(m_engine, SIGNAL(getWindowSize(int *, int *)), this, SLOT(getWindowSize(int *, int *)), Qt::DirectConnection);
    
    QObject::connect(m_cursorHideTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));

    m_engine->open("/home/lmr/24.Redemption.2008.EXTENDED.DVDRip.XviD-SAiNTS.avi");
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::toggleFullscreen()
{
    if (windowState() == Qt::WindowFullScreen) {
        m_cursorHideTimer->stop();
        showCursor();
        showNormal();
    }
    else {
        m_cursorHideTimer->start(2000);
        showFullScreen();
    }
}

void OpenGLWindow::swapBuffer()
{
    m_context->swapBuffers(this);
}

void OpenGLWindow::makeContextCurrent()
{
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        if (!m_context->isValid())
            qDebug() << "Context is not valid";
    }
    m_context->makeCurrent(this);
}

void OpenGLWindow::getWindowSize(int *width, int *height)
{
    *width = QWindow::width();
    *height = QWindow::height();
}

void OpenGLWindow::hideCursor()
{
    QCursor cursor = QWindow::cursor();
    cursor.setShape(Qt::BlankCursor);
    setCursor(cursor);
}

void OpenGLWindow::showCursor()
{
    QCursor cursor = QWindow::cursor();
    cursor.setShape(Qt::ArrowCursor);
    setCursor(cursor);
}

void OpenGLWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    event->accept();
    toggleFullscreen();
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F) {
        event->accept();
        toggleFullscreen();
    } else if (event->key() == Qt::Key_Space) {
        event->accept();
        m_engine->togglePause();
    } else if (event->key() == Qt::Key_Escape) {
        event->accept();
        if (windowState() == Qt::WindowFullScreen)
            toggleFullscreen();
        else if (Preferences::instance().getViewMode() == Preferences::VIEW_MODE_WINDOWED)
            QCoreApplication::instance()->quit();
    }
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent * event)
{
    Q_UNUSED(event)
    if (windowState() == Qt::WindowFullScreen) {           
        m_cursorHideTimer->stop();
        showCursor();
        m_cursorHideTimer->start(2000);
    }
}