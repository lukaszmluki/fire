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

PlaylistSourceAddWindow::PlaylistSourceAddWindow(const QString &name, const QString &url, QWidget *parent) :
    QDialog(parent)
{
    createUI();
    if (!name.isNull())
        m_name->setText(name);
    if (!url.isNull()) {
        m_path->setText(url);
        m_protocol->setDisabled(true);
    }
}

PlaylistSourceAddWindow::~PlaylistSourceAddWindow()
{
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
    url.setPort(m_port->text().toInt());
    qDebug() << url.toString();
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
