#ifndef SRC_TASKQUEUE_H
#define SRC_TASKQUEUE_H

#include <QObject>
#include <QQueue>
#include <QThread>

class QEvent;

class TaskQueue : public QObject
{
    Q_OBJECT
public:
    explicit TaskQueue(bool autoStartNext, QObject *parent = 0);
    ~TaskQueue();

//    static TaskQueue& instance();

    void addTask(QObject *obj, const char *member, QGenericReturnArgument ret,
                 QGenericArgument val0 = QGenericArgument(0), QGenericArgument val1 = QGenericArgument());

    bool next();

    bool event(QEvent *e);

private:
    struct Task
    {
        QObject *m_object;
        const char *m_member;
        QGenericReturnArgument m_ret;
        QGenericArgument m_val0, m_val1;
    };
    QQueue<Task> m_tasks;
    bool m_executing;
    bool m_autoStartNext;
    Qt::HANDLE m_threadId;
};

#endif /* SRC_TASKQUEUE_H */
