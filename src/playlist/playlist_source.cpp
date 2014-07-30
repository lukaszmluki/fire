/*
 * File:   playlist_source.cpp
 * Author: lmr
 *
 * Created on July 19, 2014, 08:09 PM
 */

#include "playlist_source.h"
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include "preferences.h"

namespace {
const QString CATEGORY_COMPUTER(QObject::tr("Computer"));
const QString CATEGORY_NETWORK(QObject::tr("Network"));
}

bool PlaylistSourceDetail::isNetwork() const
{
    return m_url.startsWith("ftp://") ||
           m_url.startsWith("smb://");
}

QDataStream &operator<<(QDataStream &out, const PlaylistSourceDetail &obj)
{
    out << obj.name().toUtf8().constData();
    out << obj.url().toUtf8().constData();
    return out;
}

QDataStream &operator>>(QDataStream &in, PlaylistSourceDetail &obj)
{
    char *tmp;
    in >> tmp;
    obj.setName(QString::fromUtf8(tmp));
    delete[] tmp;
    in >> tmp;
    obj.setUrl(QString::fromUtf8(tmp));
    delete[] tmp;
    return in;
}

PlaylistSource::PlaylistSource() :
    QObject(QCoreApplication::instance()),
    m_computer(platformSpecificSources())
{
    QList<PlaylistSourceDetail> user = userSources();

    Q_FOREACH(const PlaylistSourceDetail &source, user) {
        if (source.isNetwork()) {
            m_network << source;
        } else {
            m_computer << source;
        }
    }
}

PlaylistSource::~PlaylistSource()
{
}

PlaylistSource& PlaylistSource::instance()
{
    static PlaylistSource *instance = NULL;
    if (!instance) {
        qRegisterMetaType<PlaylistSourceDetail>("PlaylistSourceDetail");
        qRegisterMetaTypeStreamOperators<PlaylistSourceDetail>("PlaylistSourceDetail");
        instance = new PlaylistSource();
    }
    return *instance;
}

#ifdef Q_OS_LINUX
QList<PlaylistSourceDetail> PlaylistSource::platformSpecificSources() const
{
    QList<PlaylistSourceDetail> sources;
    sources << PlaylistSourceDetail(tr("Home"), QDir::homePath(), true)
            << PlaylistSourceDetail(tr("System"), "/", true);
    return sources;
}
#else
QList<PlaylistSourceDetail> PlaylistSource::platformSpecificSources() const
{
    return QList<PlaylistSourceDetail>();
}
#endif

QList<PlaylistSourceDetail> PlaylistSource::convertSources(const QList<QVariant> &sources)
{
    QList<PlaylistSourceDetail> result;
    Q_FOREACH(const QVariant &source, sources) {
        PlaylistSourceDetail tmp = source.value<PlaylistSourceDetail>();
        result << tmp;
    }
    return result;
}

QList<QVariant> PlaylistSource::convertSources(const QList<PlaylistSourceDetail> &sources)
{
    QList<QVariant> result;
    Q_FOREACH(const PlaylistSourceDetail &source, sources) {
        result << QVariant::fromValue(source);
    }
    return result;
}

QList<PlaylistSourceDetail> PlaylistSource::userSources() const
{
    const QVariant &v = Preferences::instance().getValue("Playlist/sources", QVariant::fromValue(QList<QVariant>()));
    return convertSources(v.toList());
}

void PlaylistSource::addNewSource(const PlaylistSourceDetail &source)
{
    if (source.isNetwork()) {
        qDebug() << "added network source";
        m_network << source;
        emit newSourceAdded(CATEGORY_NETWORK, source);
    } else {
        m_computer << source;
        emit newSourceAdded(CATEGORY_COMPUTER, source);
    }
    QList<PlaylistSourceDetail> user = userSources();
    user << source;
    Preferences::instance().setValue("Playlist/sources", convertSources(user));
}

QStringList PlaylistSource::categories() const
{
    QStringList categories(CATEGORY_COMPUTER);
    if (m_network.count())
        categories << CATEGORY_NETWORK;
    return categories;
}

QList<PlaylistSourceDetail> PlaylistSource::sources(const QString &category) const
{
    if (category == CATEGORY_COMPUTER)
        return m_computer;
    else if (category == CATEGORY_NETWORK)
        return m_network;
    return QList<PlaylistSourceDetail>();
}
