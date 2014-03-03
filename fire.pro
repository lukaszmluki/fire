TEMPLATE = app
TARGET = fire
VERSION = 0.0.1

CONFIG += debug
QT += opengl

QMAKE_CXXFLAGS += -Werror -Wextra
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS=1

INCLUDEPATH += ./external/include/

LIBS += -Lexternal/lib/ `PKG_CONFIG_PATH=./external/lib/pkgconfig pkg-config --libs libavdevice libavutil libavcodec libavfilter`

MOC_DIR = ./moc
OBJECTS_DIR = ./obj

SOURCES += src/main.cpp \
#           src/openglwindow.cpp \
           src/ffengine.cpp \
           src/systemdelegates.cpp \
           src/preferences.cpp \
           src/mainwindow.cpp \
           src/common.cpp \
           src/openglwidget.cpp \
           src/playermanager.cpp \
           src/subtitleseditor.cpp \
           contrib/libavengine/audio.c \
           contrib/libavengine/avengine.c \
           contrib/libavengine/avengine_context.c \
           contrib/libavengine/clock.c \
           contrib/libavengine/filter.c \
           contrib/libavengine/packet_queue.c \
           contrib/libavengine/system_delegate.c \
           contrib/libavengine/utils.c \
           contrib/libavengine/video.c \

HEADERS += \
#           src/openglwindow.h \
           src/ffengine.h \
           src/systemdelegates.h \
           src/preferences.h \
           src/mainwindow.h \
           src/common.h \
           src/opengldelegate.h \
           src/openglwidget.h \
           src/playermanager.h \
           src/subtitleseditor.h
