#ifndef SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H
#define SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QComboBox;
class QLabel;

class PlaylistSourceAddWindow : public QDialog
{
    Q_OBJECT
    explicit PlaylistSourceAddWindow(const QString &name,
                                     const QString &url = QString(),
                                     const QString &sourceGuid = QString(),
                                     QWidget *parent = 0);
    ~PlaylistSourceAddWindow();
public:

    static void show(const QString &name,
                     const QString &url = QString(),
                     const QString &sourceGuid = QString(),
                     QWidget *parent = 0);

signals:

private slots:
    void add();
    void protocolIndexChanged(int index);

private:
    bool validate();
    QString url() const;
    QString selectedScheme() const;
    void selectScheme(const QString &scheme);
    bool isFullUrl() const;

    QComboBox *m_protocol;
    QLineEdit *m_name;
    QLabel *m_nameLabel;
    QLineEdit *m_host;
    QLabel *m_hostLabel;
    QLineEdit *m_username;
    QLabel *m_usernameLabel;
    QLineEdit *m_password;
    QLabel *m_passwordLabel;
    QLineEdit *m_port;
    QLabel *m_portLabel;
    QLineEdit *m_path;
    QLabel *m_pathLabel;
    QString m_sourceGuid;
    void createUI();
};

#endif /* SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H */
