/*
 * File:   playlist_item.cpp
 * Author: lmr
 *
 * Created on July 16, 2014, 12:09 PM
 */

#include "playlist_item.h"
#include "playlist_data_model.h"
#include "worker.h"
#include <QDebug>

bool PlaylistItem::compare(const PlaylistItem *i1, const PlaylistItem *i2)
{
    if (i1->haveChildren() != i2->haveChildren())
        return i1->haveChildren();
    return QString(i1->name()).compare(i2->name()) > 0;
}

void PlaylistItem::addItem(PlaylistItem *item)
{
    //find position
    int i = 0;
    QList<PlaylistItem *>::iterator it = m_childItems.begin();
    while (it != m_childItems.end() && !compare(*it, item)) {
        ++it;
        ++i;
    }
    m_model->beginInsertRows(m_modelIndex, i, i);
    m_childItems.insert(it, item);
    m_model->endInsertRows();
}

int PlaylistItem::childCount()
{
    if (!m_fetched) {
        m_fetched = true;
        if (haveChildren())
            fetch();
    }
    return m_childItems.count();
}
