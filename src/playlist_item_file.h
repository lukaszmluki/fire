#ifndef PLAYLIST_ITEM_FILE_H
#define PLAYLIST_ITEM_FILE_H

#include "playlist_item.h"

class PlaylistItemFile : public PlaylistItem
{
public:
    PlaylistItemFile(const PlaylistItemData &data, PlaylistItem *parent = 0);

    PlaylistItem* child(int row);
    int childCount() const;

};

#endif // PLAYLIST_ITEM_FILE_H
