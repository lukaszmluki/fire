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
extern "C" {
#include <libavengine/avengine.h>
}

class QSize;

class FFEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FFEngine)
public:
    FFEngine(const QString &videoDevice, const QString &audioDevice, QObject *parent = 0);
    virtual ~FFEngine();

    bool openMedia(const QString &media);
    bool isMediaOpened() const;

public slots:
    void resize(const QSize &size);
    void togglePause();
    void pause();
    void resume();
    void seek(double seconds);

signals:
    // OpenGL signals
    void prepareBuffer();
    void swapBuffer();
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
};

#endif	/* SRC_FFENGINE_H */
