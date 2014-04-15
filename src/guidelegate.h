/*
 * File:   guidelegate.h
 * Author: Lukasz Marek
 *
 * Created on December 10, 2013, 10:08 PM
 */

#ifndef SRC_GUIDELEGATE_H
#define SRC_GUIDELEGATE_H

class GuiDelegate
{
public:
    virtual void muteChanged(int mute) = 0;
    virtual void volumeChanged(double volume) = 0;
};


#endif // SRC_GUIDELEGATE_H
