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

PlaylistItemFile::PlaylistItemFile(const PlaylistItemData &data, PlaylistItem *parent) :
    PlaylistItem(data, parent)
{
    av_register_all();
}

PlaylistItemFile::~PlaylistItemFile()
{
}

bool PlaylistItemFile::compare(const PlaylistItem *i1, const PlaylistItem *i2)
{
    if (i1->m_itemData.m_haveChildren != i2->m_itemData.m_haveChildren)
        return i1->m_itemData.m_haveChildren;
    return i1->m_itemData.m_name < i2->m_itemData.m_name;
}

void PlaylistItemFile::fetch()
{
    m_fetched = true;
    if (!m_itemData.m_haveChildren)
        return;
    const AVIODirEntry *next = NULL;
    AVIOContext *avioCtx;
    qDebug() << "listing... " << m_itemData.m_url;
    if (avio_open_dir(&avioCtx, m_itemData.m_url.toUtf8().constData(), NULL, NULL) >= 0) {
        while (avio_read_dir(avioCtx, &next) >= 0) {
            if (!next)
                break;
            m_childItems.push_front(new PlaylistItemFile(
                PlaylistItemData(QString::fromUtf8(next->name),
                                 QDir::cleanPath(m_itemData.m_url + "/" + next->name),
                                 next->type == AVIO_ENTRY_DIRECTORY),
                static_cast<PlaylistItem *>(this)));
        }
        avio_close_dir(avioCtx);
        qSort(m_childItems.begin(), m_childItems.end(), &compare);
    }
}

PlaylistItem* PlaylistItemFile::child(int row)
{
    return m_childItems.at(row);
}

int PlaylistItemFile::childCount()
{
    if (!m_fetched)
        fetch();
    return m_childItems.count();
}

void PlaylistItemFile::fetchMore()
{
}

bool PlaylistItemFile::canFetchMore()
{
    return false;
}
