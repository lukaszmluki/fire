/*
 * File:   utils.cpp
 * Author: Lukasz Marek
 *
 * Created on December 11, 2013, 12:09 AM
 */

#include "utils.h"

namespace Utils {

const QString APPLICATION_NAME("Fire Player");

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

}
