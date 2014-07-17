/*
 * File:   playlist_data_model.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_data_model.h"
#include "playlist_item_file.h"

PlaylistDataModel::PlaylistDataModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    m_rootItem = new PlaylistItemFile(PlaylistItemData("/", "ftp://dupa:dupadupa@localhost/", true), NULL);
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

bool PlaylistDataModel::canFetchMore(const QModelIndex &parent) const
{
    return false;
    if (!parent.isValid())
        return false;
    PlaylistItem *item = static_cast<PlaylistItem*>(parent.internalPointer());
    return item->canFetchMore();
}

void PlaylistDataModel::fetchMore (const QModelIndex &parent)
{
    if (parent.isValid()) {
        PlaylistItem *item = static_cast<PlaylistItem*>(parent.internalPointer());
        item->fetchMore();
    }
}

//void PlaylistDataModel::setupModelData(const QStringList &lines, TreeItem *parent)
//{
//     QList<TreeItem*> parents;
//     QList<int> indentations;
//     parents << parent;
//     indentations << 0;

//     int number = 0;

//     while (number < lines.count()) {
//         int position = 0;
//         while (position < lines[number].length()) {
//             if (lines[number].mid(position, 1) != " ")
//                 break;
//             position++;
//         }

//         QString lineData = lines[number].mid(position).trimmed();

//         if (!lineData.isEmpty()) {
//             // Read the column data from the rest of the line.
//             QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
//             QList<QVariant> columnData;
//             for (int column = 0; column < columnStrings.count(); ++column)
//                 columnData << columnStrings[column];

//             if (position > indentations.last()) {
//                 // The last child of the current parent is now the new parent
//                 // unless the current parent has no children.

//                 if (parents.last()->childCount() > 0) {
//                     parents << parents.last()->child(parents.last()->childCount()-1);
//                     indentations << position;
//                 }
//             } else {
//                 while (position < indentations.last() && parents.count() > 0) {
//                     parents.pop_back();
//                     indentations.pop_back();
//                 }
//             }

//             // Append a new item to the current parent's list of children.
//             parents.last()->appendChild(new TreeItem(columnData, parents.last()));
//         }

//         number++;
//     }
//}
