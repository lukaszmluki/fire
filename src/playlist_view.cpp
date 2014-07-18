/*
 * File:   playlist.cpp
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#include "playlist_view.h"
#include "playlist_data_model.h"

PlaylistView::PlaylistView(QWidget *parent) :
    QTreeView(parent)
{
    setAlternatingRowColors(true);
    setModel(new PlaylistDataModel(this));
}
