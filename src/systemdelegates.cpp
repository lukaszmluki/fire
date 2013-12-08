/*
 * File:   systemdelegates.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:57 PM
 */

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "systemdelegates.h"

namespace {

typedef void* (*thread_fun)(void *);

class FFThread : public QThread
{
public:
    FFThread(thread_fun function, void *data) : m_data(data), m_function(function) {};
    void run() { m_function(m_data); };
private:
    void *m_data;
    thread_fun m_function;
};

void qtMutexCreate(void **mutex)
{
    *mutex = new QMutex();
}

void qtMutexDestroy(void **mutex)
{
    delete static_cast<QMutex *>(*mutex);
}

void qtMutexLock(void *mutex)
{
    static_cast<QMutex *>(mutex)->lock();
}

void qtMutexUnlock(void *mutex)
{
    static_cast<QMutex *>(mutex)->unlock();
}


void qtCondCreate(void **cond)
{
    *cond = new QWaitCondition();
}

void qtCondDestroy(void **cond)
{
    delete static_cast<QWaitCondition *>(*cond);
}

void qtCondSignal(void *cond)
{
    static_cast<QWaitCondition *>(cond)->wakeOne();
}

void qtCondWait(void *cond, void *mutex)
{
    static_cast<QWaitCondition *>(cond)->wait(static_cast<QMutex *>(mutex));
}

void qtCondWaitTimeout(void *cond, void *mutex, int timeout)
{
    static_cast<QWaitCondition *>(cond)->wait(static_cast<QMutex *>(mutex), timeout);
}

void qtThreadCreate(void **thread, void* (*f)(void *), void *data)
{
    *thread = new FFThread(f, data);
    static_cast<QThread *>(*thread)->start();
}

void qtThreadDestroy(void **thread)
{
    delete static_cast<QThread *>(*thread);
}

void qtThreadWait(void *thread, int *status)
{
    Q_UNUSED(status)
    static_cast<QThread *>(thread)->wait();
}

AVSystemDelegates delegates = {
    qtMutexCreate,
    qtMutexDestroy,
    qtMutexLock,
    qtMutexUnlock,
    qtCondCreate,
    qtCondDestroy,
    qtCondSignal,
    qtCondWait,
    qtCondWaitTimeout,
    qtThreadCreate,
    qtThreadDestroy,
    qtThreadWait
};

}

namespace SystemDelegates {

AVSystemDelegates *qtSystemDelegates = &delegates;

}

