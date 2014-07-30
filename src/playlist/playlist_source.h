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
#include <QMetaType>
#include <QStringList>

class PlaylistSourceDetail
{
public:
    PlaylistSourceDetail() :
        m_fixed(false)
    {
    }

    PlaylistSourceDetail(const PlaylistSourceDetail &other) :
        m_name(other.m_name),
        m_url(other.m_url),
        m_fixed(other.m_fixed)
    {
    }

    PlaylistSourceDetail(const QString &name, const QString &url, bool fixed = false) :
        m_name(name),
        m_url(url),
        m_fixed(fixed)
    {
    }

    bool isNetwork() const;

    void setName(const QString &name)
    {
        m_name = name;
    }

    QString name() const
    {
        return m_name;
    }

    void setUrl(const QString url)
    {
        m_url = url;
    }

    QString url() const
    {
        return m_url;
    }

    bool fixed() const
    {
        return m_fixed;
    }

private:
    QString m_name;
    QString m_url;
    bool m_fixed;
};
Q_DECLARE_METATYPE(PlaylistSourceDetail)
QDataStream &operator<<(QDataStream &out, const PlaylistSourceDetail &obj);
QDataStream &operator>>(QDataStream &in, PlaylistSourceDetail &obj);

class PlaylistSource : public QObject
{
    Q_OBJECT
    PlaylistSource();
    ~PlaylistSource();
public:
    static PlaylistSource& instance();

    QStringList categories() const;
    QList<PlaylistSourceDetail> sources(const QString &category) const;

    void addNewSource(const PlaylistSourceDetail &source);

signals:
    void newSourceAdded(const QString &category, const PlaylistSourceDetail &);

private:
    static QList<QVariant> convertSources(const QList<PlaylistSourceDetail> &sources);
    static QList<PlaylistSourceDetail> convertSources(const QList<QVariant> &sources);

    QList<PlaylistSourceDetail> platformSpecificSources() const;
    QList<PlaylistSourceDetail> userSources() const;

    QList<PlaylistSourceDetail> m_computer;
    QList<PlaylistSourceDetail> m_network;

};

#endif /* SRC_PLAYLIST_SOURCE_H */
