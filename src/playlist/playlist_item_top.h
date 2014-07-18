/*
 * File:   playlist_item_top.h
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_TOP_H
#define SRC_PLAYLIST_ITEM_TOP_H

#include "playlist_item.h"

class PlaylistItemTop : public PlaylistItem
{
public:
    PlaylistItemTop(PlaylistDataModel *model);
    ~PlaylistItemTop();
};

#endif /* SRC_PLAYLIST_ITEM_TOP_H */
