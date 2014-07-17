/*
 * File:   playlist_item.cpp
 * Author: lmr
 *
 * Created on July 16, 2014, 12:09 PM
 */

#include "playlist_item.h"

#include <QThread>

//class FetchWorker : public QObject
//{
//    Q_OBJECT

//};

void PlaylistItem::asynchFetch()
{
    fetch();
}
