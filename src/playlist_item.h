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
#include <QModelIndex>

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
        m_fetchCalled(false)
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

    QModelIndex modelIndex() const
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
        return m_itemType == PLAYLIST_ITEM_DIRECTORY ;
    }

    void setName(const QString &name)
    {
        m_itemData.m_name = name;
    }

    QString name() const
    {
        return m_itemData.m_name;
    }

    virtual PlaylistItem *child(int row) = 0;
    virtual int childCount() = 0;
    virtual void fetchMore() = 0;
    virtual bool canFetchMore() = 0;

private slots:
    void fetched(void *);

protected:
    virtual void fetch(QList<PlaylistItem *> &newData) = 0;
    void asynchFetch();

    QList<PlaylistItem *> m_childItems;
    PlaylistItemData m_itemData;
    PlaylistItem *m_parentItem;
    PlaylistDataModel *m_model;
    QModelIndex m_modelIndex;
    QString m_url;
    PlaylistItemType m_itemType;

private:
    static void staticAsyncFetch(void *);
    bool m_fetchCalled;
};

#endif /* SRC_PLAYLIST_ITEM_H */
