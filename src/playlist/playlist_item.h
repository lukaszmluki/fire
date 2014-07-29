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
        PLAYLIST_ITEM_UNKNOWN,
        PLAYLIST_ITEM_DIRECTORY,
        PLAYLIST_ITEM_FILE,
        PLAYLIST_ITEM_CATEGORY
    };

    PlaylistItem(PlaylistItem *parent, PlaylistDataModel *model) :
        m_model(model),
        m_parentItem(parent),
        m_itemType(PLAYLIST_ITEM_UNKNOWN),
        m_fetched(false)
    {
        static int init = qRegisterMetaType<PlaylistItem*>("PlaylistItem*");
        Q_UNUSED(init)
    }

    virtual ~PlaylistItem()
    {
        qDeleteAll(m_childItems);
    }

    static PlaylistItem* fromUrl(const QString &url, PlaylistItem *parent, PlaylistDataModel *model);

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
        Q_ASSERT(m_itemType != PLAYLIST_ITEM_UNKNOWN);
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

    //TODO: make pure virtual
    virtual bool validate() const
    {
        return true;
    }

    //TODO: does it need to be virtual?
    int newChildPosition(const PlaylistItem *child) const;

    virtual void fetch() = 0;

    void addItem(PlaylistItem *item, int position);

protected:
    static bool compare(const PlaylistItem *i1, const PlaylistItem *i2);

    QString m_url;
    PlaylistDataModel *m_model;
    PlaylistItem *m_parentItem;
private:
    PlaylistItemType m_itemType;
    QList<PlaylistItem *> m_childItems;
    PlaylistItemData m_itemData;
    bool m_fetched;
};

#endif /* SRC_PLAYLIST_ITEM_H */
