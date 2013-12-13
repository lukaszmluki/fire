/*
 * File:   main.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#include <QGuiApplication>
#include <QApplication>
#include <QString>
#include <QFile>
#include <QDebug>
#include "openglwindow.h"
#include "mainwindow.h"
#include "preferences.h"

namespace {
const char *PARAM_WINDOWED = "--windowed";
const char *PARAM_WINDOWED_S = "-w";

void parseArgs(int argc, char **argv)
{
    //set defaults
    Preferences::instance().setViewMode(Preferences::VIEW_MODE_GUI);
    //scan params
    QStringList params;
    for (int i = 1; i < argc; i++)
        params << argv[i];

    if (params.removeAll(PARAM_WINDOWED))
        Preferences::instance().setViewMode(Preferences::VIEW_MODE_WINDOWED);
    if (params.removeAll(PARAM_WINDOWED_S))
        Preferences::instance().setViewMode(Preferences::VIEW_MODE_WINDOWED);


    Q_FOREACH(QString param, params) {
        if (QFile::exists(param)) {
            Preferences::instance().setSelectedFile(param);
            params.removeOne(param);
        }
    }
    Q_FOREACH(QString param, params) {
        qWarning() << "Unrecognized option:" << param;
    }
}
}

int main(int argc, char **argv)
{
    parseArgs(argc, argv);

    if (Preferences::instance().getViewMode() == Preferences::VIEW_MODE_WINDOWED) {
        QSurfaceFormat format;
        format.setSamples(4);
        QGuiApplication app(argc, argv);
        OpenGLWindow window;
        window.setFormat(format);
        window.resize(640, 480);
        window.show();
        return app.exec();
    } else {
        QApplication app(argc, argv);
        MainWindow window;
        return app.exec();
    }
}

