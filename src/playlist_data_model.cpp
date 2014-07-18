/*
 * File:   playlist_data_model.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_data_model.h"
#include "playlist_item_top.h"

PlaylistDataModel::PlaylistDataModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new PlaylistItemTop(this);
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

    if (role != Qt::DisplayRole)
        return QVariant();

    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());
    return item->columnData(index.column());
}

Qt::ItemFlags PlaylistDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
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
    if (childItem) {
        childItem->setModelIndex(createIndex(row, column, childItem));
        return childItem->modelIndex();
    }
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

bool PlaylistDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, count - 1);
    endInsertRows();
    return true;
}

bool PlaylistDataModel::canFetchMore(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return false;
}

void PlaylistDataModel::fetchMore (const QModelIndex &parent)
{
    Q_UNUSED(parent)
}
