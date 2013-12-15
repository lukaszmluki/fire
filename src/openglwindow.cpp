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
#include <QOpenGLContext>
#include <QCoreApplication>
#include <QTimer>
#include "utils.h"
#include "ffengine.h"
#include "preferences.h"
#include "playermanager.h"

OpenGLWindow::OpenGLWindow(QWindow *parent) :
    QWindow(parent),
    m_context(0),
    m_cursorHideTimer(new QTimer(this))
{
    qDebug("OpenGLWindow::OpenGLWindow");
    setSurfaceType(QWindow::OpenGLSurface);
    setTitle(Utils::APPLICATION_NAME);

    QObject::connect(m_cursorHideTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));
    QTimer::singleShot(0, this, SLOT(openSelectedFile()));

    PlayerManager::instance().registerPlayer(this);
}

OpenGLWindow::~OpenGLWindow()
{
}

void OpenGLWindow::toggleFullscreen()
{
    if (windowState() == Qt::WindowFullScreen) {
        m_cursorHideTimer->stop();
        showNormal();
        showCursor();
    } else {
        m_cursorHideTimer->start(2000);
        showFullScreen();
    }
}

void OpenGLWindow::swapBuffer()
{
    m_context->swapBuffers(this);
    m_context->doneCurrent();
}

void OpenGLWindow::makeContextCurrent()
{
    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();
        if (!m_context->isValid())
            qCritical("Context is not valid");
    }
    m_context->makeCurrent(this);
}

void OpenGLWindow::getWindowSize(int *width, int *height)
{
    *width = QWindow::width();
    *height = QWindow::height();
}

void OpenGLWindow::openSelectedFile()
{
    QString file = Preferences::instance().getSelectedFile();
    if (!file.isEmpty()) {
        PlayerManager::instance().getPlayer(this)->open(file);
        setTitle(file);
    }
}

void OpenGLWindow::hideCursor()
{
    QCursor cursor = QWindow::cursor();
    cursor.setShape(Qt::BlankCursor);
    setCursor(cursor);
}

void OpenGLWindow::showCursor()
{
    unsetCursor();
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
    } else if (event->key() == Qt::Key_Q) {
        PlayerManager::instance().getPlayer(this)->seek(2);
    } else if (event->key() == Qt::Key_Space) {
        event->accept();
        PlayerManager::instance().getPlayer(this)->togglePause();
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
        event->accept();
    }
}

void OpenGLWindow::fillWithColor(const QColor &color)
{

}
