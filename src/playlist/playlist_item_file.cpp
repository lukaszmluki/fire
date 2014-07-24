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

void PlaylistItemFile::fetch()
{
    QDir dir(m_url);
    PlaylistItem *item;
    const QFileInfoList &list = dir.entryInfoList(QDir::Files  | QDir::Dirs   | QDir::NoDotDot | QDir::NoDot |
                                                  QDir::Drives | QDir::Hidden | QDir::System   | QDir::Readable,
                                                  QDir::NoSort);
    Q_FOREACH(const QFileInfo &entry, list) {
        if (!entry.isDir() && !Utils::movieExtentions().contains(entry.suffix(), Qt::CaseInsensitive))
            continue;
        item = new PlaylistItemFile(static_cast<PlaylistItem *>(this), m_model);
        item->setName(entry.fileName());
        item->setUrl(m_url + (m_url.endsWith("/") ? "" : "/") + entry.fileName());
        item->setItemType(entry.isDir() ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
        addItem(item);
    }
}

bool PlaylistItemFile::validate() const
{
    //TODO
    return true;
}
