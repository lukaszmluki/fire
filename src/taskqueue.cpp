
#include "taskqueue.h"
#include <QTimer>

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

void TaskQueue::performTask()
{
    Q_ASSERT(m_threadId == QThread::currentThreadId());
    if (!m_tasks.count()) {
        m_executing = false;
        return;
    }
    const Task &task = m_tasks.dequeue();
    QMetaObject::invokeMethod(task.m_object, task.m_member, Qt::QueuedConnection,
                              task.m_ret, task.m_val0, task.m_val1);
    if (m_autoStartNext) {
        if (!m_tasks.count())
            m_executing = false;
        else
            QTimer::singleShot(0, this, SLOT(performTask()));
    }
}

void TaskQueue::addTask(QObject *obj, const char *member, QGenericReturnArgument ret,
                        QGenericArgument val0, QGenericArgument val1)
{
    Q_ASSERT(m_threadId == QThread::currentThreadId());
    m_tasks.enqueue(Task{obj, member, ret, val0, val1});
    if (!m_executing) {
        m_executing = true;
        QTimer::singleShot(0, this, SLOT(performTask()));
    }
}
