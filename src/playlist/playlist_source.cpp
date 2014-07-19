/*
 * File:   playlist_source.cpp
 * Author: lmr
 *
 * Created on July 19, 2014, 08:09 PM
 */

#include "playlist_source.h"
#include <QDir>

PlaylistSource::PlaylistSource()
{
}

PlaylistSource::~PlaylistSource()
{
}

PlaylistSource& PlaylistSource::instance()
{
    static PlaylistSource instance;
    return instance;
}

#ifdef Q_OS_LINUX
QList<PlaylistSourceDetail> PlaylistSource::platformSpecificSources() const
{
    QList<PlaylistSourceDetail> sources;
    sources << PlaylistSourceDetail(tr("Home"), QDir::homePath())
            << PlaylistSourceDetail(tr("System"), "/");
    return sources;
}
#else
QList<PlaylistSourceDetail> PlaylistSource::platformSpecificSources() const
{
    return QList<PlaylistSourceDetail>();
}
#endif

QList<PlaylistSourceCategory> PlaylistSource::getSources() const
{
    QList<PlaylistSourceCategory> result;
    QList<PlaylistSourceDetail> sources;

    sources << platformSpecificSources();

    result << PlaylistSourceCategory(tr("System"), sources);
    return result;
}
