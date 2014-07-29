/*
 * File:   playlist_item_category.h
 * Author: lmr
 *
 * Created on July 18, 2014, 08:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_CATEGORY_H
#define SRC_PLAYLIST_ITEM_CATEGORY_H

#include "playlist_item.h"

class QString;

class PlaylistItemCategory : public PlaylistItem
{
    Q_OBJECT
public:
    PlaylistItemCategory(const QString &name, PlaylistItem *parent, PlaylistDataModel *model);
private:
    void fetch();
};

#endif /* SRC_PLAYLIST_ITEM_CATEGORY_H */
