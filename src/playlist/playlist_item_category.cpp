/*
 * File:   playlist_item_category.cpp
 * Author: lmr
 *
 * Created on July 18, 2014, 08:09 PM
 */

#include "playlist_item_category.h"
#include "playlist_source.h"

#include <QString>

PlaylistItemCategory::PlaylistItemCategory(const QString &name, PlaylistItem *parent, PlaylistDataModel *model)
    : PlaylistItem(parent, model)
{
    setItemType(PLAYLIST_ITEM_CATEGORY);
    setName(name);
    connect(&PlaylistSource::instance(), SIGNAL(newSourceAdded(const QString&, const QString&, const QString&)),
            this, SLOT(addPlaylistSource(const QString&, const QString&, const QString&)));
}

void PlaylistItemCategory::addPlaylistSource(const QString &category, const QString &name, const QString &url)
{
    if (category == PlaylistItem::name()) {
        PlaylistItem *item = PlaylistItem::fromUrl(url, this, m_model);
        if (item) {
            item->setName(name);
            item->setItemType(PLAYLIST_ITEM_DIRECTORY);
            addItem(item);
        }
    }
}

void PlaylistItemCategory::fetch()
{
    QList<PlaylistSourceDetail> sources = PlaylistSource::instance().sources(name());
    Q_FOREACH(const PlaylistSourceDetail &source, sources) {
        addPlaylistSource(name(), source.m_name, source.m_url);
    }
}
