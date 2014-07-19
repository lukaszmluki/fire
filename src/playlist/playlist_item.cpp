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

class FetchData
{
public:
    FetchData(PlaylistItem *_this) : m_this(_this) {}
    PlaylistItem *m_this;
    QList<PlaylistItem *> m_newItems;
};

void PlaylistItem::fetched(void *data)
{
    FetchData *fetchData = static_cast<FetchData *>(data);
    if (fetchData->m_newItems.count()) {
        m_model->beginInsertRows(m_modelIndex, 0, fetchData->m_newItems.count() - 1);
        m_childItems = fetchData->m_newItems;
        m_model->endInsertRows();
    }
    delete fetchData;
}

void PlaylistItem::staticAsyncFetch(void *data)
{
    FetchData *fetchData = static_cast<FetchData *>(data);
    fetchData->m_this->fetch(fetchData->m_newItems);
}

void PlaylistItem::asynchFetch()
{
    if (m_fetchCalled)
        return;
    m_fetchCalled = true;
    if (haveChildren())
        Worker::shared().dispatch(staticAsyncFetch, new FetchData(this), this, SLOT(fetched(void *)));
}

void PlaylistItem::fetch(QList<PlaylistItem *> &newData)
{
    Q_UNUSED(newData)
    Q_ASSERT(false);
}

PlaylistItem* PlaylistItem::child(int row)
{
    return m_childItems.at(row);
}

int PlaylistItem::childCount()
{
    return m_childItems.count();
}
