#ifndef SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H
#define SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H

#include <QList>
#include "playlist_item.h"
#include "taskqueue.h"

class QFtp;
class QUrlInfo;

class PlaylistItemFtp : public PlaylistItem
{
    Q_OBJECT
public:
    PlaylistItemFtp(PlaylistItem *parent, PlaylistDataModel *model);
    ~PlaylistItemFtp();
private slots:
    void listInfo(const QUrlInfo &i);
    void stateChanged(int state);
    void done(bool error);
    void rawCommandReply(int code, const QString &detail);
    void fetchTask();
private:
    void fetch();

    struct Connection
    {
        QFtp *m_ftp;
        QString m_host;
        QString m_username;
        int m_port;
        QString m_path;
    };
    Connection *m_connection;

    static TaskQueue m_task;
    static QList<Connection *> m_connectionPool;

};

#endif /* SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H */
