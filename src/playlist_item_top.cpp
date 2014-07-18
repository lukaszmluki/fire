/*
 * File:   playlist_item_top.cpp
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#include "playlist_item_top.h"
#include "playlist_item_file.h"

#include <QDebug>
#include <QDir>

PlaylistItemTop::PlaylistItemTop(PlaylistDataModel *model) :
    PlaylistItem(NULL, model)
{
    setName("sources");
    PlaylistItem *item = new PlaylistItemFile(this, model);
    item->setName(tr("Home"));
    item->setItemType(PLAYLIST_ITEM_DIRECTORY);
    item->setUrl(QDir::homePath());
    m_childItems.push_back(item);
    item = new PlaylistItemFile(this, model);
    item->setName(tr("System"));
    item->setItemType(PLAYLIST_ITEM_DIRECTORY);
    item->setUrl("/");
    m_childItems.push_back(item);
}

PlaylistItemTop::~PlaylistItemTop()
{
}

PlaylistItem* PlaylistItemTop::child(int row)
{

    return m_childItems.at(row);
}

int PlaylistItemTop::childCount()
{
    return m_childItems.count();
}

void PlaylistItemTop::fetchMore()
{
}

bool PlaylistItemTop::canFetchMore()
{
    return false;
}

void PlaylistItemTop::fetch(QList<PlaylistItem *> &newData)
{
    Q_UNUSED(newData)
}
