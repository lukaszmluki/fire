/*
 * File:   playlist_item_top.h
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_TOP_H
#define SRC_PLAYLIST_ITEM_TOP_H

#include "playlist_item.h"
#include <QList>

class PlaylistItemTop : public PlaylistItem
{
public:
    PlaylistItemTop();
    ~PlaylistItemTop();

    PlaylistItem* child(int row);
    int childCount();
    void fetchMore();
    bool canFetchMore();
private:
    void fetch();
};

#endif /* SRC_PLAYLIST_ITEM_TOP_H */
