/*
 * File:   playlist.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_view.h"
#include <QMenu>
#include <QDebug>
#include <QPoint>
#include <QModelIndex>
#include "playlist_data_model.h"
#include "playlist_item.h"

PlaylistView::PlaylistView(QWidget *parent) :
    QTreeView(parent)
{
    setRootIsDecorated(false);
    setHeaderHidden(true);
    setAlternatingRowColors(true);
    setModel(new PlaylistDataModel(this));
    expandToDepth(0);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(customContextMenuRequest(const QPoint &)));
}

void PlaylistView::showDirectoryContextMenu(const QModelIndex &index, const QPoint &point) const
{
    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());
    QMenu *contextMenu = new QMenu();
    contextMenu->addAction("Add as shortcut");
    contextMenu->addAction("Add as shortcut \"" + item->name() + "\"");
    contextMenu->exec(mapToGlobal(point) + QPoint(3, 3));
    delete contextMenu;
}

void PlaylistView::showFileContextMenu(const QModelIndex &index, const QPoint &point) const
{
    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());
    Q_UNUSED(item)
    QMenu *contextMenu = new QMenu();
    contextMenu->addAction("Play");
    contextMenu->exec(mapToGlobal(point) + QPoint(3, 3));
    delete contextMenu;
}

void PlaylistView::customContextMenuRequest(const QPoint &point) const
{
    QModelIndex index = indexAt(point);
    if (index.isValid()) {
        PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());
        if (item->itemType() == PlaylistItem::PLAYLIST_ITEM_DIRECTORY)
            showDirectoryContextMenu(index, point);
        else if (item->itemType() == PlaylistItem::PLAYLIST_ITEM_FILE)
            showFileContextMenu(index, point);
    }
}
