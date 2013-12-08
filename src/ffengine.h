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

class FFEngine : public QObject
{
    Q_OBJECT
public:
    FFEngine(const QString &videoDevice, const QString &audioDevice, QObject *parent = 0);
    virtual ~FFEngine();

    bool open(const QString &media);

public slots:
    void togglePause();

signals:
    // OpenGL signals
    void beforeWriteHeader();
    void afterWriteHeader();
    void beforeWritePacket();
    void afterWritePacket();
    void beforeWriteTrailer();
    void afterWriteTrailer();
    void getWindowSize(int *, int *);
    // Player signals
    void finished();

private:
    static int initializeFFmpeg();


    // OpenGL device callbacks
    static int staticBeforeWriteHeaderCallback(struct AVFormatContext *ctx, void *extra);
    static int staticAfterWriteHeaderCallback(struct AVFormatContext *ctx, void *extra);
    static int staticBeforeWritePacketCallback(struct AVFormatContext *ctx, void *extra);
    static int staticAfterWritePacketCallback(struct AVFormatContext *ctx, void *extra);
    static int staticBeforeWriteTrailerCallback(struct AVFormatContext *ctx, void *extra);
    static int staticAfterWriteTrailerCallback(struct AVFormatContext *ctx, void *extra);
    static int staticWindowSizeCallback(struct AVFormatContext *ctx, int *width, int *height);
    int beforeWriteHeaderCallback();
    int afterWriteHeaderCallback();
    int beforeWritePacketCallback();
    int afterWritePacketCallback();
    int beforeWriteTrailerCallback();
    int afterWriteTrailerCallback();
    int windowSizeCallback(int *width, int *height);

    // Player callbacks
    static void staticFinishedCallback(AVEngineContext *ctx);
    static void staticAudioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx);
    static void staticVideoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx);
    void finishedCallback(AVEngineContext *ctx);
    void audioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx);
    void videoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx);

    bool createContext();
    void freeContext();

    static AVEngineCallback m_staticCallbacks;
    AVEngineContext *m_AVEngineContext;
    QString m_videoDevice;
    QString m_audioDevice;
};

#endif	/* SRC_FFENGINE_H */
