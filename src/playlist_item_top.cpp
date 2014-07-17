/*
 * File:   playlist_item_top.cpp
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#include "playlist_item_top.h"
#include "playlist_item_file.h"

#include <QDebug>

PlaylistItemTop::PlaylistItemTop() :
    PlaylistItem(PlaylistItemData("Sources", "", true), NULL)
{
    m_childItems.push_back(new PlaylistItemFile(PlaylistItemData("ftp dupa", "ftp://dupa:dupadupa@localhost/", true), this));
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

void PlaylistItemTop::fetch()
{
}
