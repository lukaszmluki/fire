/*
 * File:   playlist_item_top.cpp
 * Author: lmr
 *
 * Created on July 17, 2014, 08:09 PM
 */

#include "playlist_item_top.h"
#include <QDebug>
#include "playlist_item_category.h"
#include "playlist_source.h"
#include "playlist_data_model.h"

PlaylistItemTop::PlaylistItemTop(PlaylistDataModel *model) :
    PlaylistItem(NULL, model)
{
    setName("sources");
    setItemType(PLAYLIST_ITEM_DIRECTORY);
}

PlaylistItemTop::~PlaylistItemTop()
{
}

void PlaylistItemTop::fetch()
{
    QStringList categories = PlaylistSource::instance().categories();
    Q_FOREACH(const QString &category, categories) {
        m_model->addItem(this, new PlaylistItemCategory(category, this, m_model));
    }
}
