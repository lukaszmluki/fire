/*
 * File:   playlist.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_H
#define SRC_PLAYLIST_H

#include <QTreeView>

class QModelIndex;

class PlaylistView : public QTreeView
{
    Q_OBJECT
public:
    explicit PlaylistView(QWidget *parent = 0);

private:
    void showDirectoryContextMenu(const QModelIndex &index, const QPoint &point) const;
    void showFileContextMenu(const QModelIndex &index, const QPoint &point) const;
    void addShortcut(const QString &url, const QString &name);

signals:

public slots:

private slots:
    void customContextMenuRequest(const QPoint &point) const;
    void addShortcut();
    void addShortcutNamed();
    void addNewSource();
    void removeShortcut();
    void editShortcut();
};

#endif /* SRC_PLAYLIST_H */
