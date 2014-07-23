#include "playlist_item_ftp.h"
#include <QDebug>
#include <QUrl>
#include "common.h"

PlaylistItemFtp::PlaylistItemFtp(PlaylistItem *parent, PlaylistDataModel *model) :
    PlaylistItem(parent, model),
    m_ftp(new QFtp(this))
{
    connect(m_ftp, SIGNAL(listInfo(const QUrlInfo &)), this, SLOT(listInfo(const QUrlInfo &)));
    connect(m_ftp, SIGNAL(done(bool)), this, SLOT(done(bool)));
    connect(m_ftp, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
    connect(m_ftp, SIGNAL(rawCommandReply(int, const QString &)), this, SLOT(rawCommandReply(int, const QString &)));

}

PlaylistItemFtp::~PlaylistItemFtp()
{
}

void PlaylistItemFtp::stateChanged(int state)
{
    Q_UNUSED(state)
    //qDebug() << "state" << state;
}

void PlaylistItemFtp::done(bool error)
{
    Q_UNUSED(error)
    //qDebug() << "done" << error;
}

void PlaylistItemFtp::rawCommandReply(int code, const QString &detail)
{
    if (code == 257) {
        QUrl url(m_url);
        QString path = detail.mid(1, detail.indexOf("\"", 1) - 1);
        path.append('/').append(url.path());
        m_ftp->list(path);
    }
}

void PlaylistItemFtp::listInfo(const QUrlInfo &i)
{
    QString utf8Name = QString::fromUtf8(i.name().toStdString().c_str());
    QString ext;
    int pos = utf8Name.lastIndexOf(".");
    if (pos != -1)
        ext = utf8Name.mid(pos + 1);
    if (!i.isDir() && !Utils::movieExtentions().contains(ext, Qt::CaseInsensitive))
        return;
    PlaylistItemFtp *item = new PlaylistItemFtp(static_cast<PlaylistItem *>(this), m_model);
    item->setName(utf8Name);
    item->setUrl(m_url + (m_url.endsWith("/") ? "" : "/") + utf8Name);
    item->setItemType(i.isDir() ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
    addItem(item);
}

void PlaylistItemFtp::fetch()
{
    QUrl url(m_url);
    m_ftp->connectToHost(url.host(), url.port());
    m_ftp->login(url.userName(), url.password());
    m_ftp->rawCommand("PWD");
}
