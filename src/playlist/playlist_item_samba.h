#ifndef SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H
#define SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H

#include "playlist_item.h"
#include <libsmbclient.h>

class PlaylistItemSamba : public PlaylistItem
{
public:
    PlaylistItemSamba(PlaylistItem *parent, PlaylistDataModel *model);
    ~PlaylistItemSamba();
private:
    bool connect();
    static void auth(SMBCCTX *c, const char *server, const char *share,
                     char *workgroup, int workgroup_len,
                     char *username, int username_len,
                     char *password, int password_len);
    void fetch();
    void privateFetch();
    static void statichFetch(void *data);

    SMBCCTX *m_ctx;
signals:
    void gotItem(PlaylistItem *);
};

#endif /* SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H */
