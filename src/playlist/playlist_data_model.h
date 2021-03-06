/*
 * File:   playlist_data_model.h
 * Author: lmr
 *
 * Created on July 15, 2014, 12:09 PM
 */

#ifndef SRC_PLAYLIST_DATA_MODEL_H
#define SRC_PLAYLIST_DATA_MODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "playlist_source.h"

class PlaylistItem;

class PlaylistDataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PlaylistDataModel(QObject *parent = 0);
    virtual ~PlaylistDataModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

public slots:
    void addPlaylistSource(const QString &category, const PlaylistSourceDetail &source);
    void addItem(PlaylistItem *parent, PlaylistItem *child);
    void removeSource(const QString &guid);

private:
    QModelIndex findIndex(PlaylistItem *item) const;
    PlaylistItem *m_rootItem;
};

#endif /* SRC_PLAYLIST_DATA_MODEL_H */
