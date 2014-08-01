
#include "playlist_item_samba.h"
#include <QDebug>
#include <QMetaObject>
#include "worker.h"
#include "playlist_data_model.h"

void PlaylistItemSamba::auth(SMBCCTX *c, const char *server, const char *share,
                             char *workgroup, int workgroupLength,
                             char *username, int usernameLength,
                             char *password, int passwordLength)
{
    Q_UNUSED(c)
    Q_UNUSED(server) Q_UNUSED(share)
    Q_UNUSED(workgroup) Q_UNUSED(workgroupLength)
    Q_UNUSED(username)  Q_UNUSED(usernameLength)
    Q_UNUSED(password) Q_UNUSED(passwordLength)
    /* Do nothing yet. Credentials are passed via url.
     * Callback must exists, there might be a segmentation fault otherwise. */
}

bool PlaylistItemSamba::connect()
{
    m_ctx = smbc_new_context();
    if (!m_ctx) {
        qCritical() << "Cannot create context:" << strerror(errno);
        return false;
    }
    if (!smbc_init_context(m_ctx)) {
        qCritical() << "Cannot initialize context:" << strerror(errno);
        return false;
    }
    smbc_set_context(m_ctx);

    smbc_setOptionUserData(m_ctx, this);
    smbc_setFunctionAuthDataWithContext(m_ctx, &auth);
    smbc_setTimeout(m_ctx, 5000);

    if (smbc_init(NULL, 0) < 0) {
        qCritical() << "Initialization failed:" << strerror(errno);
        return false;
    }
    return true;
}

PlaylistItemSamba::PlaylistItemSamba(PlaylistItem *parent, PlaylistDataModel *model) :
    PlaylistItem(parent, model)
{
}

PlaylistItemSamba::~PlaylistItemSamba()
{
}

void PlaylistItemSamba::privateFetch()
{
    if (connect()) {
        struct smbc_dirent *entry;
        int dh;
        PlaylistItem *item;
        const QString &url = PlaylistItem::url();
        if ((dh = smbc_opendir(url.toUtf8().constData())) < 0) {
            qCritical() << "Cannot open directory:" << strerror(errno);
            return;
        }
        do {
            if (!(entry = smbc_readdir(dh)))
                break;
            if (!strcmp(entry->name, ".") || !strcmp(entry->name, ".."))
                continue;
            item = new PlaylistItemSamba(static_cast<PlaylistItem *>(this), m_model);
            switch (entry->smbc_type) {
            case SMBC_DIR:
            case SMBC_FILE_SHARE:
                item->setItemType(PLAYLIST_ITEM_DIRECTORY);
                break;
            case SMBC_FILE:
                item->setItemType(PLAYLIST_ITEM_FILE);
                break;
            default:
                delete item;
                continue;
            }
            item->setName(QString::fromUtf8(entry->name));
            item->setUrl(url + (url.endsWith("/") ? "" : "/") + item->name());
            QMetaObject::invokeMethod(m_model, "addItem", Qt::QueuedConnection, QGenericReturnArgument(),
                                      Q_ARG(PlaylistItem *, this), Q_ARG(PlaylistItem *, item));

        } while (true);
    }
}

void PlaylistItemSamba::statichFetch(void *data)
{
    PlaylistItemSamba *_this = static_cast<PlaylistItemSamba *>(data);
    _this->privateFetch();
}


void PlaylistItemSamba::fetch()
{
    Worker::instance().dispatch(statichFetch, this);
}
