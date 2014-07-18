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
#include "guidelegate.h"

class QSize;
class QRect;

struct AVFilterGraph;
struct AVFilterContext;
struct AVFormatContext;

class FFEngine : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(FFEngine)
public:
    static bool initializeFFmpeg();

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
    // private signals
    void requestPacket(int streamIndex);

private slots:
    void pushPacket(int streamIndex);

private:
    static int staticControlMessage(struct AVFormatContext *ctx, int type,
                                    void *data, size_t size);

    bool createDevice();
    bool probeDevice();
    bool openDevice();
    bool createStream();
    bool createSink(struct AVFilterContext *filter, int idx);
    bool createFilterGraph();

private:
    /* FFmpeg stuff */
    struct AVFormatContext *m_deviceContext;
    struct AVFilterGraph *m_filterGraph;
    struct AVFilterContext *m_movieFilter;
    struct AVFilterContext *m_buffersinkFilter;
    struct AVFilterContext *m_abuffersinkFilter;

    int m_audioStreamIndex;
    int m_videoStreamIndex;
    QString m_mediaUrl;
    QString m_videoDeviceName;
    QString m_audioDeviceName;
    DeviceOptions m_videoDeviceOptions;
    DeviceOptions m_audioDeviceOptions;
};

#endif	/* SRC_FFENGINE_H */
