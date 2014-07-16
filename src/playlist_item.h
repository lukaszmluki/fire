/*
 * File:   playlist_item.h
 * Author: lmr
 *
 * Created on July 16, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_ITEM_H
#define SRC_PLAYLIST_ITEM_H

#include <QList>
#include <QVariant>
#include <QString>

class PlaylistItemData
{
public:
    PlaylistItemData(const QString &name, const QString &url) :
        m_name(name),
        m_url(url)
    {
    }

    int columnCount() const
    {
        return 1;
    }
    QVariant columntData(int column) const
    {
        Q_ASSERT(column < columnCount());
        return QVariant(m_name);
    }

    QString m_name;
    QString m_url;
};

class PlaylistItem
{
public:
    PlaylistItem(const PlaylistItemData &data, PlaylistItem *parent = 0) :
        m_itemData(data),
        m_parentItem(parent)
    {
    }

    virtual ~PlaylistItem()
    {
    }

    int columnCount() const
    {
        return m_itemData.columnCount();
    }

    PlaylistItem *parent()
    {
        return m_parentItem;
    }

    QVariant columnData(int column) const
    {
        return m_itemData.columntData(column);
    }

    int row() const
    {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<PlaylistItem *>(this));
        return 0;
    }

    virtual PlaylistItem *child(int row) = 0;
    virtual int childCount() const = 0;

private:
    QList<PlaylistItem *> m_childItems;
    PlaylistItemData m_itemData;
    PlaylistItem *m_parentItem;
};

#endif /* SRC_PLAYLIST_ITEM_H */
