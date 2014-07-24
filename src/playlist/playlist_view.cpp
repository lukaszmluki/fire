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
#include "window/playlist_source_add_window.h"

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

void PlaylistView::addShortcut(const QString &url, const QString &name)
{
    PlaylistSourceAddWindow *dialog = new PlaylistSourceAddWindow(name, url, this);
    dialog->exec();
    dialog->deleteLater();
}

void PlaylistView::addNewSource()
{
    addShortcut(QString(), QString());
}

void PlaylistView::addShortcut()
{
    QVariant data = dynamic_cast<QAction *>(sender())->data();
    const PlaylistItem *item = static_cast<PlaylistItem *>(data.value<void*>());
    addShortcut(item->url(), QString());
}

void PlaylistView::addShortcutNamed()
{
    QVariant data = dynamic_cast<QAction *>(sender())->data();
    const PlaylistItem *item = static_cast<PlaylistItem *>(data.value<void*>());
    addShortcut(item->url(), item->name());
}

void PlaylistView::showDirectoryContextMenu(const QModelIndex &index, const QPoint &point) const
{
    PlaylistItem *item = static_cast<PlaylistItem *>(index.internalPointer());
    QAction *a;
    QMenu *contextMenu = new QMenu();

    contextMenu->addAction("Add new source", this, SLOT(addNewSource()));

    if (item->parent()->itemType() != PlaylistItem::PLAYLIST_ITEM_CATEGORY) {
        contextMenu->addSeparator();
        a = contextMenu->addAction("Add as shortcut", this, SLOT(addShortcut()));
        a->setData(QVariant::fromValue(index.internalPointer()));
        a = contextMenu->addAction("Add as shortcut \"" + item->name() + "\"", this, SLOT(addShortcutNamed()));
        a->setData(QVariant::fromValue(index.internalPointer()));
    }

    contextMenu->exec(mapToGlobal(point) + QPoint(3, 3));
    contextMenu->deleteLater();
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
