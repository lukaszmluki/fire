/*
 * File:   main.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#include <QString>
#include <QGuiApplication>
#include "openglwindow.h"
#include "preferences.h"

namespace {
const char *PARAM_WINDOWED = "--windowed";

void parseArgs(int argc, char **argv)
{
    //set defaults
    Preferences::instance().setViewMode(Preferences::VIEW_MODE_WINDOWED);
    //scan params
    for (int i = 0; i < argc; i++) {
        QString param(argv[i]);
        if (param == PARAM_WINDOWED)
            Preferences::instance().setViewMode(Preferences::VIEW_MODE_WINDOWED);
    }
}
}

int main(int argc, char **argv)
{
    parseArgs(argc, argv);
    QSurfaceFormat format;
    format.setSamples(4);

    if (Preferences::instance().getViewMode() == Preferences::VIEW_MODE_WINDOWED) {
        QGuiApplication app(argc, argv);
        OpenGLWindow window;
        window.setFormat(format);
        window.resize(640, 480);
        window.show();
        return app.exec();
    }
}

