#ifndef SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H
#define SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H

#include "playlist_item.h"
#include <QFtp>

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
private:
    void fetch();
    QFtp *m_ftp;
};

#endif /* SRC_PLAYLIST_PLAYLIST_ITEM_FTP_H */
