#ifndef SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H
#define SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H

#include <QDialog>

class QLineEdit;

class PlaylistSourceAddWindow : public QDialog
{
    Q_OBJECT
public:
    explicit PlaylistSourceAddWindow(const QString &name, const QString &url = QString(), QWidget *parent = 0);
    ~PlaylistSourceAddWindow();

signals:

private slots:
    void add();

private:
    QLineEdit *m_name;
    QLineEdit *m_host;
    QLineEdit *m_username;
    QLineEdit *m_password;
    QLineEdit *m_port;
    QLineEdit *m_path;
    void createUI();
};

#endif /* SRC_WINDOW_PLAYLIST_SOURCE_ADD_WINDOW_H */
