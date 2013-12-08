
MOC_DIR = ./moc
OBJECTS_DIR = ./obj

QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS=1

INCLUDEPATH += ./external/include/

LIBS += -Lexternal/lib/ `PKG_CONFIG_PATH=./external/lib/pkgconfig pkg-config --libs libavdevice libavutil libavcodec libavfilter libavengine`

SOURCES += src/main.cpp \
           src/openglwindow.cpp \
           src/ffengine.cpp \
           src/systemdelegates.cpp \
           src/preferences.cpp

HEADERS += src/openglwindow.h \
           src/ffengine.h \
           src/systemdelegates.h \
           src/preferences.h
