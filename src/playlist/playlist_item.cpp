/*
 * File:   playlist_item.cpp
 * Author: lmr
 *
 * Created on July 16, 2014, 12:09 PM
 */

#include "playlist_item.h"
#include <QDebug>
#include "playlist_data_model.h"
#include "playlist_item_file.h"
#include "playlist_item_ftp.h"
#include "playlist_item_samba.h"

PlaylistItem* PlaylistItem::fromUrl(const QString &url, PlaylistItem *parent, PlaylistDataModel *model)
{
    PlaylistItem *item = NULL;
    if (url.startsWith("ftp://", Qt::CaseInsensitive)) {
        item = new PlaylistItemFtp(parent, model);
    } else if (url.startsWith("smb://", Qt::CaseInsensitive)) {
#ifdef HAVE_SMBCLIENT
        item = new PlaylistItemSamba(parent, model);
#else
        qWarning() << "Compiled without samba support.";
#endif
    } else {
        item = new PlaylistItemFile(parent, model);
    }
    if (item)
        item->setUrl(url);
    return item;
}

bool PlaylistItem::compare(const PlaylistItem *i1, const PlaylistItem *i2)
{
    if (i1->fixed() != i2->fixed())
        return i2->fixed();
    if (i1->haveChildren() != i2->haveChildren())
        return i2->haveChildren();
    return QString(i1->name()).compare(i2->name()) > 0;
}

int PlaylistItem::newChildPosition(const PlaylistItem *child) const
{
    QList<PlaylistItem *>::const_iterator it = m_childItems.begin();
    int i = 0;
    while (it != m_childItems.end() && !compare(*it, child)) {
        ++i;
        ++it;
    }
    return i;
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
