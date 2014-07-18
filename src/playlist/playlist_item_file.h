/*
 * File:   playlist_item_file.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_FILE_H
#define SRC_PLAYLIST_ITEM_FILE_H

#include "playlist_item.h"
#include <QList>

class PlaylistItemFile : public PlaylistItem
{
public:
    PlaylistItemFile(PlaylistItem *parent, PlaylistDataModel *model);
    ~PlaylistItemFile();

    PlaylistItem* child(int row);
    int childCount();
    void fetchMore();
    bool canFetchMore();
private:
    static bool compare(const PlaylistItem *i1, const PlaylistItem *i2);
    void fetch(QList<PlaylistItem *> &newData);
};

#endif /* SRC_PLAYLIST_ITEM_FILE_H */
