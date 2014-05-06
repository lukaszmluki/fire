TEMPLATE = app
TARGET = fire
VERSION = 0.0.1

CONFIG += debug
QT += opengl

QMAKE_CXXFLAGS += -Werror -Wextra
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS=1

INCLUDEPATH += ./external/include/

LIBS += -Lexternal/lib/ `PKG_CONFIG_PATH=./external/lib/pkgconfig pkg-config --libs libavdevicefire libavutilfire libavcodecfire libavfilterfire`

MOC_DIR = ./moc
OBJECTS_DIR = ./obj

SOURCES += src/main.cpp \
#           src/openglwindow.cpp \
           src/ffengine.cpp \
           src/preferences.cpp \
           src/mainwindow.cpp \
           src/common.cpp \
           src/openglwidget.cpp \
           src/playermanager.cpp \
           src/subtitleseditor.cpp \
           src/x11widget.cpp \
           src/ffmpeg/ffavdictionary.cpp

HEADERS += \
#           src/openglwindow.h \
           src/ffengine.h \
           src/preferences.h \
           src/mainwindow.h \
           src/common.h \
           src/videowidget.h \
           src/openglwidget.h \
           src/playermanager.h \
           src/subtitleseditor.h \
           src/x11widget.h \
           src/ffmpeg/ffavdictionary.h
