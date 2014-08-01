#include "playlist_source_add_window.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFont>
#include <QComboBox>
#include <QUrl>
#include <QMessageBox>
#include "playlist/playlist_item.h"
#include "playlist/playlist_source.h"

enum class ProtocolType { FILE, FTP, SAMBA };

void PlaylistSourceAddWindow::createUI()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    setLayout(layout);
    setWindowTitle(tr("Add new source"));

    auto addTextField = [&layout](QLabel* &label, QLineEdit* &widget, const QString &placeholder)
    {
        QFont font;
        label = new QLabel(placeholder);
        font = label->font();
        font.setPointSize(font.pointSize() * 0.75f);
        label->setFont(font);
        layout->addWidget(label, layout->count());
        widget = new QLineEdit();
        widget->setAlignment(Qt::AlignCenter);
        widget->setPlaceholderText(placeholder);
        layout->addWidget(widget, layout->count());
    };

    m_protocol = new QComboBox();
    m_protocol->addItem(tr("Local directory"), QVariant(static_cast<int>(ProtocolType::FILE)));
    m_protocol->addItem(tr("FTP (File Transfer Protocol"), QVariant(static_cast<int>(ProtocolType::FTP)));
#ifdef HAVE_SMBCLIENT
    m_protocol->addItem(tr("Samba (SMB/CIFS)"), QVariant(static_cast<int>(ProtocolType::SAMBA)));
#endif
    connect(m_protocol, SIGNAL(currentIndexChanged(int)), this, SLOT(protocolIndexChanged(int)));
    layout->addWidget(m_protocol, layout->count());
    layout->addSpacing(6);

    addTextField(m_nameLabel, m_name, tr("Name"));
    addTextField(m_hostLabel, m_host, tr("Host"));
    addTextField(m_usernameLabel, m_username, tr("Username"));
    addTextField(m_passwordLabel, m_password, tr("Password"));
    m_password->setEchoMode(QLineEdit::Password);
    addTextField(m_portLabel, m_port, tr("Port"));
    m_port->setValidator(new QIntValidator(1, 65535, this));
    addTextField(m_pathLabel, m_path, tr("Path"));

    layout->addSpacing(6);
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel);
    buttons->setMinimumWidth(380);
    connect(buttons, SIGNAL(accepted()), this, SLOT(add()));
    connect(buttons, SIGNAL(rejected()), dynamic_cast<QDialog *>(this), SLOT(reject()));
    layout->addWidget(buttons, layout->count());
    layout->setSizeConstraint(QLayout::SetFixedSize);
    protocolIndexChanged(m_protocol->currentIndex());
}

PlaylistSourceAddWindow::PlaylistSourceAddWindow(const QString &name, const QString &url, const QString &sourceGuid, QWidget *parent) :
    QDialog(parent),
    m_sourceGuid(sourceGuid)
{
    createUI();
    if (!name.isNull())
        m_name->setText(name);
    if (!url.isNull()) {
        QUrl lurl(url);
        selectScheme(lurl.scheme());
        m_protocol->setDisabled(true);
        m_path->setText(lurl.path());
        if (lurl.port() > 0)
            m_port->setText(QString::number(lurl.port()));
        m_host->setText(lurl.host());
        m_username->setText(lurl.userName());
        m_password->setText(lurl.password());
    }
}

PlaylistSourceAddWindow::~PlaylistSourceAddWindow()
{
}

void PlaylistSourceAddWindow::show(const QString &name, const QString &url, const QString &sourceGuid, QWidget *parent)
{
    PlaylistSourceAddWindow *dialog = new PlaylistSourceAddWindow(name, url, sourceGuid, parent);
    dialog->exec();
    dialog->deleteLater();
}

QString PlaylistSourceAddWindow::selectedScheme() const
{
    switch(static_cast<ProtocolType>(m_protocol->itemData(m_protocol->currentIndex()).toInt())) {
    case ProtocolType::FILE:
        return "file";
    case ProtocolType::FTP:
        return "ftp";
    case ProtocolType::SAMBA:
        return "smb";
    default:
        break;
    }
    return QString();
}

void PlaylistSourceAddWindow::selectScheme(const QString &scheme)
{
    ProtocolType protocol = ProtocolType::FILE;
    if (!scheme.compare("ftp", Qt::CaseInsensitive))
        protocol = ProtocolType::FTP;
    else if (!scheme.compare("smb", Qt::CaseInsensitive))
        protocol = ProtocolType::SAMBA;
    int cnt = m_protocol->count();
    for (int i = 0; i < cnt; ++i) {
        if (static_cast<ProtocolType>(m_protocol->itemData(i).toInt()) == protocol) {
            m_protocol->setCurrentIndex(i);
            return;
        }
    }
    m_protocol->setCurrentIndex(0);
}

QString PlaylistSourceAddWindow::url() const
{
    if (!isFullUrl())
        return m_path->text();
    QUrl url;
    url.setHost(m_host->text());
    url.setPath(m_path->text());
    url.setPassword(m_password->text());
    url.setUserName(m_username->text());
    if (m_port->text().length())
        url.setPort(m_port->text().toInt());
    url.setScheme(selectedScheme());
    return url.toString();
}

bool PlaylistSourceAddWindow::validate()
{
    QSharedPointer<PlaylistItem> pi = QSharedPointer<PlaylistItem>(PlaylistItem::fromUrl(url(), NULL, NULL));
    return pi->validate();
}

void PlaylistSourceAddWindow::add()
{
    if (m_name->text().isEmpty()) {
        QMessageBox::critical(this, tr("Missing name"), tr("Name field is mandatory."));
        return;
    }
    if (!validate()) {
        QMessageBox::critical(this, tr("Invalid data"), tr("Provided data is invalid."));
        return;
    }
    if (!m_sourceGuid.isNull())
        PlaylistSource::instance().updateSource(PlaylistSourceDetail(m_name->text(), url()), m_sourceGuid);
    else
        PlaylistSource::instance().addNewSource(PlaylistSourceDetail(m_name->text(), url()));
    accept();
}

bool PlaylistSourceAddWindow::isFullUrl() const
{
    return m_protocol->itemData(m_protocol->currentIndex()) != static_cast<int>(ProtocolType::FILE);
}

void PlaylistSourceAddWindow::protocolIndexChanged(int index)
{
    Q_UNUSED(index)
    bool hideNetworkDetails = !isFullUrl();
    m_host->setHidden(hideNetworkDetails);
    m_hostLabel->setHidden(hideNetworkDetails);
    m_username->setHidden(hideNetworkDetails);
    m_usernameLabel->setHidden(hideNetworkDetails);
    m_password->setHidden(hideNetworkDetails);
    m_passwordLabel->setHidden(hideNetworkDetails);
    m_port->setHidden(hideNetworkDetails);
    m_portLabel->setHidden(hideNetworkDetails);
}
