/*
 * File:   ffengine.h
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#ifndef SRC_FFENGINE_H
#define	SRC_FFENGINE_H

#include <QtCore/QObject>

struct AVEngineContext;

class FFEngine : public QObject
{
    Q_OBJECT
public:
    FFEngine();
    virtual ~FFEngine();

    bool open(const QString &media);
private:
    static int initializeFFmpeg();
    bool createContext();
    void freeContext();

    AVEngineContext *m_AVEngineContext;
};

#endif	/* SRC_FFENGINE_H */
