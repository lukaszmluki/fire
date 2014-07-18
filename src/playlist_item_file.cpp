/*
 * File:   playlist_item_file.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_item_file.h"
#include <QDebug>
#include <QDir>
extern "C" {
#include <libavformat/avio.h>
#include <libavformat/avformat.h>
}

PlaylistItemFile::PlaylistItemFile(const PlaylistItemData &data, PlaylistItem *parent, PlaylistDataModel *model) :
    PlaylistItem(data, parent, model)
{
}

PlaylistItemFile::~PlaylistItemFile()
{
}

bool PlaylistItemFile::compare(const PlaylistItem *i1, const PlaylistItem *i2)
{
    if (i1->itemData().m_haveChildren != i2->itemData().m_haveChildren)
        return i1->itemData().m_haveChildren;
    return i1->itemData().m_name < i2->itemData().m_name;
}

void PlaylistItemFile::fetch(QList<PlaylistItem *> &newData)
{
    const AVIODirEntry *next = NULL;
    AVIOContext *avioCtx;
    QString childUrl;
    qDebug() << "listing... " << m_itemData.m_url;
    if (avio_open_dir(&avioCtx, m_itemData.m_url.toUtf8().constData(), NULL, NULL) >= 0) {
        while ((avio_read_dir(avioCtx, &next) >= 0) && next) {
            childUrl = m_itemData.m_url + (m_itemData.m_url.endsWith("/") ? "" : "/") + next->name;
            newData.push_front(new PlaylistItemFile(
                PlaylistItemData(QString::fromUtf8(next->name), childUrl, next->type == AVIO_ENTRY_DIRECTORY),
                static_cast<PlaylistItem *>(this), m_model));
        }
        avio_close_dir(avioCtx);
        qSort(newData.begin(), newData.end(), &compare);
    }
}

PlaylistItem* PlaylistItemFile::child(int row)
{
    return m_childItems.at(row);
}

int PlaylistItemFile::childCount()
{
    asynchFetch();
    return m_childItems.count();
}

void PlaylistItemFile::fetchMore()
{
}

bool PlaylistItemFile::canFetchMore()
{
    return false;
}
