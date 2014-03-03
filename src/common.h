/*
 * File:   utils.h
 * Author: Lukasz Marek
 *
 * Created on December 11, 2013, 12:09 AM
 */

#ifndef SRC_UTILS_H
#define	SRC_UTILS_H

#include <QString>
class QWidget;

namespace Utils {

extern const QString APPLICATION_NAME;

QString imagePath(const QString &file);

QString selectVideo(QWidget *parent = 0);

}

#endif	/* UTILS_H */

