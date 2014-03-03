/*
 * File:   utils.cpp
 * Author: Lukasz Marek
 *
 * Created on December 11, 2013, 12:09 AM
 */

#include "common.h"
#include <QFileDialog>
#include <QDir>
#include "preferences.h"

namespace Utils {

const QString APPLICATION_NAME("Fire Player");
const QString VIDEO_FILTER = QObject::tr("Video files *.avi *.mp4 *.mkv *.mpeg *.mpg *.wmv;;All files (*.*)");

QString imagePath(const QString &file)
{
#if defined Q_OS_LINUX
    static QString base("./images/");
    return base + file;
#elif defined Q_OS_WIN32
    static QString base("images\\");
    return base + file;
#endif
    return file;
}

QString selectVideo(QWidget *parent)
{
    return QFileDialog::getOpenFileName(
        parent, QObject::tr("Select movie..."),
        Preferences::instance().getValue("last_dir", QDir::homePath()).toString(),
        VIDEO_FILTER);

//    QFileDialog dialog(parent, QObject::tr("Select Video..."), QDir::homePath());
//    dialog.setModal(true);
//    dialog.setFileMode(QFileDialog::QFileDialog::ExistingFile);
//    dialog.setViewMode(QFileDialog::Detail);
//    dialog.setNameFilter(VIDEO_FILTER);
//    dialog.exec();
//    return dialog.selectedFiles().value(0);
}

}
