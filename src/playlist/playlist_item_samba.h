#ifndef SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H
#define SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H

#include "playlist_item.h"

class PlaylistItemSamba : public PlaylistItem
{
public:
    PlaylistItemSamba(PlaylistItem *parent, PlaylistDataModel *model);
    ~PlaylistItemSamba();
private:
    void fetch();
};

#endif /* SRC_PLAYLIST_PLAYLIST_ITEM_SAMBA_H */
