#include "playlist_source_add_window.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QDebug>
#include <QFont>

void PlaylistSourceAddWindow::createUI()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    setLayout(layout);
    setWindowTitle(tr("Add new source"));

    auto addField = [&layout](QLineEdit* &widget, const QString &placeholder) {
        QLabel *label;
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
        layout->addSpacing(6);
    };

    addField(m_name, tr("Name"));
    addField(m_host, tr("Host"));
    addField(m_username, tr("Username"));
    addField(m_password, tr("Password"));
    addField(m_port, tr("Port"));
    addField(m_path, tr("Path"));

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                     QDialogButtonBox::Cancel);
    buttons->setMinimumWidth(380);
    connect(buttons, SIGNAL(accepted()), this, SLOT(add()));
    connect(buttons, SIGNAL(rejected()), dynamic_cast<QDialog *>(this), SLOT(reject()));
    layout->addWidget(buttons, layout->count());
    layout->setSizeConstraint(QLayout::SetFixedSize);
}

PlaylistSourceAddWindow::PlaylistSourceAddWindow(const QString &name, const QString &url, QWidget *parent) :
    QDialog(parent)
{
    createUI();
    m_name->setText(name);
    m_path->setText(url);
    m_password->setEchoMode(QLineEdit::Password);
}

PlaylistSourceAddWindow::~PlaylistSourceAddWindow()
{
}

void PlaylistSourceAddWindow::add()
{
    qDebug("add");
}
