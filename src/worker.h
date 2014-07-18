/*
 * File:   worker.h
 * Author: lmr
 *
 * Created on July 17, 2014, 08:29 PM
 */

#ifndef SRC_WORKER_H
#define SRC_WORKER_H

#include <QObject>
#include <QEvent>
#include <QDebug>

class QThread;
class WorkerObject;

class Worker : public QObject
{
    Q_OBJECT

    Worker();
    ~Worker();

public:

    typedef void (*DispatchFunction)(void *);

    static Worker& shared();

    void dispatch(DispatchFunction f, void *data, QObject *receiver = NULL, const char *method = NULL);

private:
    QThread *m_thread;
    WorkerObject *m_worker;
};


//Private classes

class WorkerObject : public QObject
{
    Q_OBJECT

    WorkerObject(bool selfDelete) :
        m_selfDelete(selfDelete)
    {
    }
    ~WorkerObject()
    {
    }

public:
    bool event(QEvent *e);

signals:
    void finished(void *);

private:
    bool m_selfDelete;

friend class Worker;
};

class DispatchEvent : public QEvent
{
    DispatchEvent(Worker::DispatchFunction f, void *data) :
        QEvent(m_eventId),
        m_f(f),
        m_data(data)
    {
    }
    ~DispatchEvent() {}
public:

    Type type() const
    {
        return m_eventId;
    }

    static const Type m_eventId;

    Worker::DispatchFunction m_f;
    void *m_data;

friend class WorkerObject;
friend class Worker;
};


#endif /* SRC_WORKER_H */
