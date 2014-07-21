/*
 * File:   playlist_item_file.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_item_file.h"
#include <QDebug>
#include <QDir>
#include "common.h"

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
    QDir dir(m_url);
    PlaylistItem *item;
    qDebug() << "listing... " << m_url;
    const QFileInfoList &list = dir.entryInfoList(QDir::Files  | QDir::Dirs   | QDir::NoDotDot | QDir::NoDot |
                                                  QDir::Drives | QDir::Hidden | QDir::System   | QDir::Readable,
                                                  QDir::DirsFirst | QDir::Name);
    Q_FOREACH(const QFileInfo &entry, list) {
        if (!entry.isDir() && !Utils::movieExtentions().contains(entry.suffix(), Qt::CaseInsensitive))
            continue;
        item = new PlaylistItemFile(static_cast<PlaylistItem *>(this), m_model);
        item->setName(entry.fileName());
        item->setUrl(m_url + (m_url.endsWith("/") ? "" : "/") + entry.fileName());
        item->setItemType(entry.isDir() ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
        newData.push_back(item);
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
