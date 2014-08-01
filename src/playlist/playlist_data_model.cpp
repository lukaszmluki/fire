/*
 * File:   playlist_data_model.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_data_model.h"
#include <QDebug>
#include <QIcon>
#include <QQueue>
#include "common.h"
#include "playlist_item_top.h"
#include "playlist_source.h"
#include "playlist_item_category.h"

PlaylistDataModel::PlaylistDataModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new PlaylistItemTop(this);
    connect(&PlaylistSource::instance(), SIGNAL(newSourceAdded(const QString&, const PlaylistSourceDetail &)),
            this, SLOT(addPlaylistSource(const QString&, const PlaylistSourceDetail &)));
}

PlaylistDataModel::~PlaylistDataModel()
{
    delete m_rootItem;
}

int PlaylistDataModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<PlaylistItem *>(parent.internalPointer())->columnCount();
    else
        return m_rootItem->columnCount();
 }

QVariant PlaylistDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        return item->columnData(index.column());
    } else if (role == Qt::DecorationRole && item->itemType() == PlaylistItem::PLAYLIST_ITEM_DIRECTORY) {
        return QIcon(Utils::imagePath("playlist/dir.png"));
    } else if (role == Qt::TextAlignmentRole && item->itemType() == PlaylistItem::PLAYLIST_ITEM_CATEGORY) {
        return Qt::AlignCenter;
    }
    return QVariant();
}

Qt::ItemFlags PlaylistDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());

    if (item->itemType() == PlaylistItem::PLAYLIST_ITEM_CATEGORY)
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant PlaylistDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
       return m_rootItem->columnData(section);
    return QVariant();
}

QModelIndex PlaylistDataModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    PlaylistItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());

    PlaylistItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex PlaylistDataModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    PlaylistItem *childItem = static_cast<PlaylistItem*>(index.internalPointer());
    PlaylistItem *parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
 }

int PlaylistDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    PlaylistItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<PlaylistItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QModelIndex PlaylistDataModel::findIndex(PlaylistItem *item) const
{
    if (item == m_rootItem)
        return QModelIndex();
    QQueue<QModelIndex> queue;
    int i;
    PlaylistItem *localitem;
    queue.enqueue(QModelIndex());
    while (queue.count()) {
        const QModelIndex &parent = queue.dequeue();
        if (parent.internalPointer() == item)
            return parent;
        i = rowCount(parent);
        for (int j = 0; j < i; ++j) {
            QModelIndex ix = index(j, 0, parent);
            localitem = static_cast<PlaylistItem*>(ix.internalPointer());
            if (localitem->itemType() == PlaylistItem::PLAYLIST_ITEM_FILE ||
                (localitem->itemType() == PlaylistItem::PLAYLIST_ITEM_DIRECTORY &&
                 !item->url().startsWith(localitem->url()))) {
                continue;
            }
            queue.enqueue(ix);
        }
    }
    return QModelIndex();
}

void PlaylistDataModel::addItem(PlaylistItem *parent, PlaylistItem *child)
{
    int position = parent->newChildPosition(child);
    beginInsertRows(findIndex(parent), position, position);
    parent->addItem(child, position);
    endInsertRows();
}

void PlaylistDataModel::addPlaylistSource(const QString &category, const PlaylistSourceDetail &source)
{
    PlaylistItem *parentItem = NULL;
    qDebug() << m_rootItem->childCount();
    for (int i = 0; i < m_rootItem->childCount(); ++i) {
        PlaylistItem *item = m_rootItem->child(i);
        qDebug() << item->name();
        if (item->name() == category) {
            parentItem = item;
            break;
        }
    }
    if (!parentItem) {
        qDebug() << "Category" << category << "not found. Adding.";
        parentItem = new PlaylistItemCategory(category, m_rootItem, this);
        addItem(m_rootItem, parentItem);
    }

    PlaylistItem *item = PlaylistItem::fromUrl(source.url(), parentItem, this);
    if (item) {
        item->setName(source.name());
        item->setFixed(source.fixed());
        item->setItemType(PlaylistItem::PLAYLIST_ITEM_DIRECTORY);
        addItem(parentItem, item);
    }
}
