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
#include <QPersistentModelIndex>

class PlaylistDataModel;

class PlaylistItemData
{
public:
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
};

class PlaylistItem : public QObject
{
    Q_OBJECT
public:

    enum PlaylistItemType {
        PLAYLIST_ITEM_DIRECTORY,
        PLAYLIST_ITEM_FILE,
        PLAYLIST_ITEM_CATEGORY
    };

    PlaylistItem(PlaylistItem *parent, PlaylistDataModel *model) :
        m_parentItem(parent),
        m_model(model),
        m_fetched(false)
    {
    }

    virtual ~PlaylistItem()
    {
        qDeleteAll(m_childItems);
    }

    int columnCount() const
    {
        return m_itemData.columnCount();
    }

    PlaylistItem *parent() const
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

    void setModelIndex(const QModelIndex &modelIndex)
    {
        m_modelIndex = modelIndex;
    }

    QPersistentModelIndex modelIndex() const
    {
        return m_modelIndex;
    }

    void setUrl(const QString url)
    {
        m_url = url;
    }

    QString url() const
    {
        return m_url;
    }

    void setItemType(PlaylistItemType type)
    {
        m_itemType = type;
    }

    PlaylistItemType itemType() const
    {
        return m_itemType;
    }

    bool haveChildren() const
    {
        return m_itemType == PLAYLIST_ITEM_DIRECTORY ||
               m_itemType == PLAYLIST_ITEM_CATEGORY;
    }

    void setName(const QString &name)
    {
        m_itemData.m_name = name;
    }

    QString name() const
    {
        return m_itemData.m_name;
    }

    PlaylistItem *child(int row)
    {
        return m_childItems.at(row);
    }

    int childCount();

    virtual void fetch() = 0;

protected slots:
    void addItem(PlaylistItem *item);

protected:
    static bool compare(const PlaylistItem *i1, const PlaylistItem *i2);

    QList<PlaylistItem *> m_childItems;
    PlaylistItemData m_itemData;
    PlaylistItem *m_parentItem;
    PlaylistDataModel *m_model;
    QPersistentModelIndex m_modelIndex;
    QString m_url;
    PlaylistItemType m_itemType;
private:
    bool m_fetched;
};

#endif /* SRC_PLAYLIST_ITEM_H */
