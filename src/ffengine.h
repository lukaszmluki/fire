/*
 * File:   ffengine.h
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#ifndef SRC_FFENGINE_H
#define	SRC_FFENGINE_H

#include <QObject>
#include <QString>
#include <QMap>
extern "C" {
#include <libavengine/avengine.h>
}
#include "guidelegate.h"

class QSize;
class QRect;

class FFEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FFEngine)
public:
    typedef QMap<QString, QString> DeviceOptions;

    FFEngine(const QString &videoDevice, const QString &audioDevice,
             const DeviceOptions &videoOptions, const DeviceOptions &audioOptions,
             QObject *parent = 0);
    virtual ~FFEngine();

    bool openMedia(const QString &media);
    bool isMediaOpened() const;

    bool addGuiDelegate(GuiDelegate *guiDelegate);

public slots:
    void resize(const QSize &size);
    void repaint(const QRect &area);
    void togglePause();
    void pause();
    void resume();
    void seek(double seconds);
    void setVolume(double volume);
    void toggleMute();

signals:
    // OpenGL signals
    void prepareBuffer();
    void swapBuffer();
    // Audio device signals
    void volumeChanged(double volume);
    void muteChanged(int mute);
    // Player signals
    void finished();
    void paused();
    void resumed();
    void durationChanged(double duration);
    void positionChanged(double position);

private:
    static int initializeFFmpeg();

    static int staticControlMessage(struct AVFormatContext *ctx, int type,
                                    void *data, size_t size);

    // Player callbacks
    static void staticFinishedCallback(AVEngineContext *ctx);
    static void staticPauseChangedCallback(AVEngineContext *ctx);
    static void staticPositionChanged(AVEngineContext *ctx);
    static void staticDurationChanged(AVEngineContext *ctx);
    static void staticAudioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx);
    static void staticVideoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx);
    void finishedCallback();
    void pauseChangedCallback();
    void positionChanged();
    void durationChanged();
    void audioOutputContextCreatedCallback(AVFormatContext *actx);
    void videoOutputContextCreatedCallback(AVFormatContext *vctx);

    bool createContext();
    void freeContext();

    static AVEngineCallback m_staticCallbacks;
    AVEngineContext *m_AVEngineContext;
    QString m_videoDevice;
    QString m_audioDevice;
    DeviceOptions m_videoOptions;
    DeviceOptions m_audioOptions;
};

#endif	/* SRC_FFENGINE_H */
