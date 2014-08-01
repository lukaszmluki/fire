
#include "taskqueue.h"
#include <QEvent>
#include <QCoreApplication>

class TaskEvent : public QEvent
{
public:
    TaskEvent() :
        QEvent(m_eventId)
    {

    }
    static const Type m_eventId;
};
const QEvent::Type TaskEvent::m_eventId = static_cast<QEvent::Type>(QEvent::registerEventType());

TaskQueue::TaskQueue(bool autoStartNext, QObject *parent) :
    QObject(parent),
    m_autoStartNext(autoStartNext),
    m_threadId(QThread::currentThreadId())
{
}

TaskQueue::~TaskQueue()
{
}

//TaskQueue& TaskQueue::instance()
//{
//    static TaskQueue instance;
//    return instance;
//}

bool TaskQueue::event(QEvent *e)
{
    if (e->type() == TaskEvent::m_eventId) {
        Q_ASSERT(m_threadId == QThread::currentThreadId());
        const Task &task = m_tasks.dequeue();
        QMetaObject::invokeMethod(task.m_object, task.m_member, Qt::QueuedConnection,
                                  task.m_ret, task.m_val0, task.m_val1);
        if (m_autoStartNext)
            next();
    }
    return QObject::event(e);
}

bool TaskQueue::next()
{
    Q_ASSERT(m_threadId == QThread::currentThreadId());
    if (!m_tasks.count())
        m_executing = false;
    else
        QCoreApplication::instance()->postEvent(this, new TaskEvent(), -10);
    return m_executing;
}

void TaskQueue::addTask(QObject *obj, const char *member, QGenericReturnArgument ret,
                        QGenericArgument val0, QGenericArgument val1)
{
    Q_ASSERT(m_threadId == QThread::currentThreadId());
    m_tasks.enqueue(Task{obj, member, ret, val0, val1});
    if (!m_executing) {
        m_executing = true;
        QCoreApplication::instance()->postEvent(this, new TaskEvent(), -10);
    }
}
