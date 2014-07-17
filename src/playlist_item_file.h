/*
 * File:   playlist_item_file.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef PLAYLIST_ITEM_FILE_H
#define PLAYLIST_ITEM_FILE_H

#include "playlist_item.h"
#include <QList>

class PlaylistItemFile : public PlaylistItem
{
public:
    PlaylistItemFile(const PlaylistItemData &data, PlaylistItem *parent = 0);
    ~PlaylistItemFile();

    PlaylistItem* child(int row);
    int childCount();
    void fetchMore();
    bool canFetchMore();
private:
    static bool compare(const PlaylistItem *i1, const PlaylistItem *i2);
    void fetch();
};

#endif // PLAYLIST_ITEM_FILE_H
