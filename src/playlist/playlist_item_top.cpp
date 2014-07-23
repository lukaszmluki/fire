/*
 * File:   playlist_item_top.cpp
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#include "playlist_item_top.h"
#include <QDebug>
#include "playlist_item_file.h"
#include "playlist_item_ftp.h"
#include "playlist_item_category.h"
#include "playlist_source.h"

PlaylistItemTop::PlaylistItemTop(PlaylistDataModel *model) :
    PlaylistItem(NULL, model)
{
    setName("sources");
    PlaylistItem *item;
    QList<PlaylistSourceCategory> sources = PlaylistSource::instance().getSources();
    Q_FOREACH(const PlaylistSourceCategory &category, sources) {
        PlaylistItemCategory *newCategory;
        newCategory = new PlaylistItemCategory(category.m_category, this, model);
        m_childItems.push_back(newCategory);
        Q_FOREACH(const PlaylistSourceDetail &source, category.m_sources) {
            if (source.m_url.startsWith("ftp", Qt::CaseInsensitive)) {
                item = new PlaylistItemFtp(newCategory, model);
            } else {
                item = new PlaylistItemFile(newCategory, model);
            }
            item->setName(source.m_name);
            item->setUrl(source.m_url);
            item->setItemType(PLAYLIST_ITEM_DIRECTORY);
            newCategory->addPlaylistItem(item);
        }
    }
}

PlaylistItemTop::~PlaylistItemTop()
{
}

void PlaylistItemTop::fetch()
{
}
