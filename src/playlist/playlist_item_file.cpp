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

PlaylistItemFile::PlaylistItemFile(PlaylistItem *parent, PlaylistDataModel *model) :
    PlaylistItem(parent, model)
{
}

PlaylistItemFile::~PlaylistItemFile()
{
}

bool PlaylistItemFile::compare(const PlaylistItem *i1, const PlaylistItem *i2)
{
    if (i1->haveChildren() != i2->haveChildren())
        return i1->haveChildren();
    return i1->name() < i2->name();
}

void PlaylistItemFile::fetch(QList<PlaylistItem *> &newData)
{
    const AVIODirEntry *next = NULL;
    AVIOContext *avioCtx;
    PlaylistItem *item;
    qDebug() << "listing... " << m_url;
    if (avio_open_dir(&avioCtx, url().toUtf8().constData(), NULL, NULL) >= 0) {
        while ((avio_read_dir(avioCtx, &next) >= 0) && next) {
            item = new PlaylistItemFile(static_cast<PlaylistItem *>(this), m_model);
            item->setName(QString::fromUtf8(next->name));
            item->setUrl(m_url + (m_url.endsWith("/") ? "" : "/") + next->name);
            item->setItemType(next->type == AVIO_ENTRY_DIRECTORY ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
            newData.push_front(item);
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
