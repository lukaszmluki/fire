#include <QtGui/QGuiApplication>
#include "openglwindow.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(4);

    OpenGLWindow window;
    window.setFormat(format);
    window.resize(640, 480);
    window.show();

    return app.exec();
}

