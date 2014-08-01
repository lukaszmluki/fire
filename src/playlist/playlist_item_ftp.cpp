#include "playlist_item_ftp.h"
#include <QDebug>
#include <QUrl>
#include "common.h"
#include "playlist_data_model.h"

TaskQueue PlaylistItemFtp::m_task(false);
QList<QSharedPointer<PlaylistItemFtp::Connection>> PlaylistItemFtp::m_connectionPool;

PlaylistItemFtp::PlaylistItemFtp(PlaylistItem *parent, PlaylistDataModel *model) :
    PlaylistItem(parent, model),
    m_connection(NULL)
{
}

PlaylistItemFtp::~PlaylistItemFtp()
{
}

void PlaylistItemFtp::stateChanged(int state)
{
    Q_UNUSED(state)
    //qDebug() << "state" << state;
}

void PlaylistItemFtp::done(bool error)
{
    //qDebug() << "done" << error;
    disconnect(m_connection->m_ftp, 0, this, 0);
    if (!error)
        m_connectionPool.prepend(m_connection);
    m_connection.clear();
    m_task.next();
}

void PlaylistItemFtp::fetchTask()
{
    QUrl url(PlaylistItem::url());

    auto it = m_connectionPool.begin();
    for(; it != m_connectionPool.end(); ++it) {
        if ((*it)->m_host == url.host() &&
            (*it)->m_port == url.port() &&
            (*it)->m_username == url.userName())
        {
            if ((*it)->m_ftp->state() != QFtp::LoggedIn) {
                it = m_connectionPool.erase(it);
                continue;
            }
            m_connection = *it;
            m_connectionPool.erase(it);
            connect(m_connection->m_ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(listInfo(const QUrlInfo &)));
            connect(m_connection->m_ftp, SIGNAL(done(bool)), this, SLOT(done(bool)));
            connect(m_connection->m_ftp, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
            connect(m_connection->m_ftp, SIGNAL(rawCommandReply(int, const QString &)), this, SLOT(rawCommandReply(int, const QString &)));
            break;
        }
    }

    if (!m_connection) {
        m_connection = QSharedPointer<Connection>(new Connection());
        m_connection->m_ftp = new QFtp();
        m_connection->m_host = url.host();
        m_connection->m_port = url.port();
        m_connection->m_username = url.userName();
        connect(m_connection->m_ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(listInfo(const QUrlInfo &)));
        connect(m_connection->m_ftp, SIGNAL(done(bool)), this, SLOT(done(bool)));
        connect(m_connection->m_ftp, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
        connect(m_connection->m_ftp, SIGNAL(rawCommandReply(int, const QString &)), this, SLOT(rawCommandReply(int, const QString &)));
        m_connection->m_ftp->connectToHost(url.host(), url.port());
        m_connection->m_ftp->login(url.userName(), url.password());
        m_connection->m_ftp->rawCommand("PWD");
    } else {
        QString tmp = m_connection->m_path;
        m_connection->m_ftp->list(tmp.append('/').append(url.path()));
    }
}

void PlaylistItemFtp::rawCommandReply(int code, const QString &detail)
{
    if (code == 257) {
        QUrl url(PlaylistItem::url());
        m_connection->m_path = detail.mid(1, detail.indexOf("\"", 1) - 1);
        QString tmp = m_connection->m_path;
        m_connection->m_ftp->list(tmp.append('/').append(url.path()));
    } else {
        qWarning() << "Cannot stat remore directory";
        m_connection.clear();
        m_task.next();
    }
}

void PlaylistItemFtp::listInfo(const QUrlInfo &i)
{
    const QString &url = PlaylistItem::url();
    QString utf8Name = QString::fromUtf8(i.name().toStdString().c_str());
    QString ext;
    int pos = utf8Name.lastIndexOf(".");
    if (pos != -1)
        ext = utf8Name.mid(pos + 1);
    if (!i.isDir() && !Utils::movieExtentions().contains(ext, Qt::CaseInsensitive))
        return;
    PlaylistItemFtp *item = new PlaylistItemFtp(static_cast<PlaylistItem *>(this), m_model);
    item->setName(utf8Name);
    item->setUrl(url + (url.endsWith("/") ? "" : "/") + utf8Name);
    item->setItemType(i.isDir() ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
    m_model->addItem(this, item);
}

void PlaylistItemFtp::fetch()
{
    m_task.addTask(this, "fetchTask", QGenericReturnArgument());
}
