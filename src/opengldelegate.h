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
public:
    virtual void prepareBuffer() = 0;
    virtual void swapBuffer() = 0;
    virtual int windowWidth() = 0;
    virtual int windowHeight() = 0;
};

#endif /* SRC_OPENGL_DELEGATE_H */
