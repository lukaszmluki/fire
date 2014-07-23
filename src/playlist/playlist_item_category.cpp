/*
 * File:   playlist_item_category.cpp
 * Author: lmr
 *
 * Created on July 18, 2014, 08:09 PM
 */

#include "playlist_item_category.h"

#include <QString>

PlaylistItemCategory::PlaylistItemCategory(const QString &name, PlaylistItem *parent, PlaylistDataModel *model)
    : PlaylistItem(parent, model)
{
    setItemType(PLAYLIST_ITEM_CATEGORY);
    setName(name);
}

void PlaylistItemCategory::addPlaylistItem(PlaylistItem *item)
{
    m_childItems.push_back(item);
}

void PlaylistItemCategory::fetch()
{
}
