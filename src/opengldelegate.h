/*
 * File:   opengldelegate.h
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:42 AM
 */

#ifndef SRC_OPENGL_DELEGATE_H
#define SRC_OPENGL_DELEGATE_H

#include <QColor>

class OpenGLDelegate
{
    virtual void fillWithColor(const QColor &color = Qt::black) = 0;
    virtual void moveContextToDeviceThread() = 0;
    virtual void moveContextToMainThread() = 0;
    virtual void swapBuffer() = 0;
    virtual void makeContextCurrent() = 0;
    virtual void getWindowSize(int *width, int *height) = 0;
};

#endif /* SRC_OPENGL_DELEGATE_H */
