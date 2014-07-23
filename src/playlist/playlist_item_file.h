/*
 * File:   playlist_item_file.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_FILE_H
#define SRC_PLAYLIST_ITEM_FILE_H

#include "playlist_item.h"

class PlaylistItemFile : public PlaylistItem
{
public:
    PlaylistItemFile(PlaylistItem *parent, PlaylistDataModel *model);
    ~PlaylistItemFile();
private:
    void fetch();
};

#endif /* SRC_PLAYLIST_ITEM_FILE_H */
