/*
 * File:   playlist_item_category.cpp
 * Author: lmr
 *
 * Created on July 18, 2014, 08:09 PM
 */

#include "playlist_item_category.h"
#include <QString>
#include <QDebug>
#include "playlist_source.h"
#include "playlist_data_model.h"

PlaylistItemCategory::PlaylistItemCategory(const QString &name, PlaylistItem *parent, PlaylistDataModel *model)
    : PlaylistItem(parent, model)
{
    setItemType(PLAYLIST_ITEM_CATEGORY);
    setName(name);
}

void PlaylistItemCategory::fetch()
{
    QList<PlaylistSourceDetail> sources = PlaylistSource::instance().sources(name());
    Q_FOREACH(const PlaylistSourceDetail &source, sources) {
        m_model->addPlaylistSource(name(), source);
    }
}
