/*
 * File:   opengldelegate.h
 * Author: Lukasz Marek
 *
 * Created on December 12, 2013, 10:42 AM
 */

#ifndef SRC_VIDEO_WIDGET_H
#define SRC_VIDEO_WIDGET_H

#include <QString>

class VideoWidget
{
public:
    virtual ~VideoWidget() {}

    virtual void prepareBuffer() = 0;
    virtual void swapBuffer() = 0;
    virtual int windowWidth() = 0;
    virtual int windowHeight() = 0;
    virtual QString engineName() = 0;
};

#endif /* SRC_VIDEO_WIDGET_H */
