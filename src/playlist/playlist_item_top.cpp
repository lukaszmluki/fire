/*
 * File:   playlist_item_top.cpp
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#include "playlist_item_top.h"
#include <QDebug>
#include <QDir>
#include "playlist_item_file.h"
#include "playlist_item_category.h"

PlaylistItemTop::PlaylistItemTop(PlaylistDataModel *model) :
    PlaylistItem(NULL, model)
{
    setName("sources");

    PlaylistItemCategory *category;
    category = new PlaylistItemCategory(tr("System"), this, model);
    m_childItems.push_back(category);

    PlaylistItem *item = new PlaylistItemFile(category, model);
    item->setName(tr("Home"));
    item->setItemType(PLAYLIST_ITEM_DIRECTORY);
    item->setUrl(QDir::homePath());
    category->addPlaylistItem(item);

    item = new PlaylistItemFile(category, model);
    item->setName(tr("System"));
    item->setItemType(PLAYLIST_ITEM_DIRECTORY);
    item->setUrl("/");
    category->addPlaylistItem(item);
}

PlaylistItemTop::~PlaylistItemTop()
{
}
