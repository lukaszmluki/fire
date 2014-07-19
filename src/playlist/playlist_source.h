/*
 * File:   playlist_source.cpp
 * Author: lmr
 *
 * Created on July 19, 2014, 08:09 PM
 */

#ifndef SRC_PLAYLIST_SOURCE_H
#define SRC_PLAYLIST_SOURCE_H

#include <QList>
#include <QString>
#include <QObject>

class PlaylistSourceDetail
{
public:
    PlaylistSourceDetail(const QString &name, const QString &url) :
        m_name(name),
        m_url(url)
    {
    }

    QString m_name;
    QString m_url;
};

class PlaylistSourceCategory
{
public:
    PlaylistSourceCategory(const QString &category, const QList<PlaylistSourceDetail> &sources) :
        m_category(category),
        m_sources(sources)
    {
    }

    QString m_category;
    QList<PlaylistSourceDetail> m_sources;
};

class PlaylistSource : public QObject
{
    Q_OBJECT
    PlaylistSource();
    ~PlaylistSource();
public:
    static PlaylistSource& instance();
    QList<PlaylistSourceCategory> getSources() const;

private:
    QList<PlaylistSourceDetail> platformSpecificSources() const;
};

#endif /* SRC_PLAYLIST_SOURCE_H */
