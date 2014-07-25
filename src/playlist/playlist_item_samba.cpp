
#include "playlist_item_samba.h"
#include <QDebug>
#include <QMetaObject>
#include "worker.h"

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
        if ((dh = smbc_opendir(m_url.toUtf8().constData())) < 0) {
            qCritical() << "Cannot open directory:" << strerror(errno);
            return;
        }
        do {
            if (!(entry = smbc_readdir(dh)))
                break;
            if (!strcmp(entry->name, ".") || !strcmp(entry->name, ".."))
                continue;
            item = new PlaylistItemSamba(static_cast<PlaylistItem *>(this), m_model);
            item->setName(QString::fromUtf8(entry->name));
            item->setUrl(m_url + (m_url.endsWith("/") ? "" : "/") + item->name());
            item->setItemType(entry->smbc_type == SMBC_DIR ? PLAYLIST_ITEM_DIRECTORY : PLAYLIST_ITEM_FILE);
            QMetaObject::invokeMethod(this, "addItem", Qt::QueuedConnection,
                                      QGenericReturnArgument(), Q_ARG(PlaylistItem *, item));

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
