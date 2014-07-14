/*
 * File:   playlist.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_H
#define SRC_PLAYLIST_H

#include <QTreeView>

class PlaylistView : public QTreeView
{
    Q_OBJECT
public:
    explicit PlaylistView(QWidget *parent = 0);

signals:

public slots:

};

#endif /* SRC_PLAYLIST_H */
