/*
 * File:   worker.h
 * Author: lmr
 *
 * Created on July 17, 2014, 08:29 PM
 */

#include "worker.h"

#include <QThread>
#include <QCoreApplication>

const QEvent::Type DispatchEvent::m_eventId = static_cast<QEvent::Type>(QEvent::registerEventType());

bool WorkerObject::event(QEvent *e)
{
    if (e->type() == DispatchEvent::m_eventId) {
        DispatchEvent *ev = static_cast<DispatchEvent *>(e);
        ev->m_f(ev->m_data);
        emit finished(ev->m_data);
        if (m_selfDelete)
            deleteLater();
        return true;
    }
    return false;
}

Worker::Worker()
{
    m_thread = new QThread();
    m_thread->start();
    m_worker = new WorkerObject(false);
    m_worker->moveToThread(m_thread);
}

Worker::~Worker()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
    delete m_worker;
}

Worker& Worker::shared()
{
    static Worker instance;
    return instance;
}

void Worker::dispatch(DispatchFunction f, void *data, QObject *receiver, const char *method)
{
    WorkerObject *worker = m_worker;
    if (receiver && method) {
        worker = new WorkerObject(true);
        worker->moveToThread(m_thread);
        QObject::connect(worker, SIGNAL(finished(void *)), receiver, method, Qt::QueuedConnection);
    }
    QCoreApplication::postEvent(worker, new DispatchEvent(f, data));
}
