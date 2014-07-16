#include "playlist_item_file.h"

PlaylistItemFile::PlaylistItemFile(const PlaylistItemData &data, PlaylistItem *parent) :
    PlaylistItem(data, parent)
{
}

PlaylistItem* PlaylistItemFile::child(int row)
{
    return new PlaylistItemFile(PlaylistItemData(QString::number(row), QString()), this);
}

int PlaylistItemFile::childCount() const
{
    return 2;
}
