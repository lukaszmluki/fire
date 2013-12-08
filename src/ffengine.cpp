/*
 * File:   ffengine.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#include <QDebug>
extern "C" {
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
}
#include "ffengine.h"
#include "systemdelegates.h"

AVEngineCallback FFEngine::m_staticCallbacks = {
    FFEngine::staticFinishedCallback,
    FFEngine::staticAudioOutputContextCreatedCallback,
    FFEngine::staticVideoOutputContextCreatedCallback
};

int FFEngine::initializeFFmpeg()
{
    av_register_all();
    avdevice_register_all();
    avfilter_register_all();
    avformat_network_init();
    if (avengine_init(SystemDelegates::qtSystemDelegates) < 0)
        return 0;
    av_log_set_level(AV_LOG_DEBUG);
    return 1;
}

FFEngine::FFEngine(const QString &videoDevice, const QString &audioDevice, QObject *parent) :
    QObject(parent),
    m_AVEngineContext(NULL),
    m_videoDevice(videoDevice),
    m_audioDevice(audioDevice)
{
    static int init = initializeFFmpeg();
    Q_UNUSED(init)
}

FFEngine::~FFEngine()
{
    freeContext();
}

int FFEngine::staticBeforeWriteHeaderCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->beforeWriteHeaderCallback();
}

int FFEngine::staticAfterWriteHeaderCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->afterWriteHeaderCallback();
}

int FFEngine::staticBeforeWritePacketCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->beforeWritePacketCallback();
}

int FFEngine::staticAfterWritePacketCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->afterWritePacketCallback();
}

int FFEngine::staticBeforeWriteTrailerCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->beforeWriteTrailerCallback();
}

int FFEngine::staticAfterWriteTrailerCallback(struct AVFormatContext *ctx, void *extra)
{
    Q_UNUSED(extra);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->afterWriteTrailerCallback();
}

int FFEngine::staticWindowSizeCallback(struct AVFormatContext *ctx, int *width, int *height)
{
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_user_data(ctx));
    return _this->windowSizeCallback(width, height);
}

int FFEngine::beforeWriteHeaderCallback()
{
    emit beforeWriteHeader();
    return 0;
}

int FFEngine::afterWriteHeaderCallback()
{
    emit afterWriteHeader();
    return 0;
}

int FFEngine::beforeWritePacketCallback()
{
    emit  beforeWritePacket();
    return 0;
}

int FFEngine::afterWritePacketCallback()
{
    emit afterWritePacket();
    return 0;
}

int FFEngine::beforeWriteTrailerCallback()
{
    emit beforeWriteTrailer();
    return 0;
}

int FFEngine::afterWriteTrailerCallback()
{
    emit afterWriteTrailer();
    return 0;
}

int FFEngine::windowSizeCallback(int *width, int *height)
{
    emit getWindowSize(width, height);
    return 0;
}

void FFEngine::staticFinishedCallback(AVEngineContext *ctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->finishedCallback(ctx);
}

void FFEngine::staticAudioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->audioOutputContextCreatedCallback(ctx, actx);
}

void FFEngine::staticVideoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->videoOutputContextCreatedCallback(ctx, vctx);
}

void FFEngine::finishedCallback(AVEngineContext *ctx)
{
    Q_UNUSED(ctx);
    qDebug() << "finishedCallback";
}

void FFEngine::audioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx)
{
    Q_UNUSED(ctx);
    qDebug() << "audioOutputContextCreatedCallback";
    av_format_set_user_data(actx, this);
}

void FFEngine::videoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx)
{
    Q_UNUSED(ctx);
    qDebug() << "videoOutputContextCreatedCallback";
    //TODO: this should be done by engine API
    av_format_set_user_data(vctx, this);
    av_format_set_before_write_header_cb(vctx, staticBeforeWriteHeaderCallback);
    av_format_set_after_write_header_cb(vctx, staticAfterWriteHeaderCallback);
    av_format_set_before_write_packet_cb(vctx, staticBeforeWritePacketCallback);
    av_format_set_after_write_packet_cb(vctx, staticAfterWritePacketCallback);
    av_format_set_before_write_trailer_cb(vctx, staticBeforeWriteTrailerCallback);
    av_format_set_after_write_trailer_cb(vctx, staticAfterWriteTrailerCallback);
    av_format_set_window_size_cb(vctx, staticWindowSizeCallback);
}

void FFEngine::freeContext()
{
    avengine_close_media(m_AVEngineContext);
    avengine_context_free(&m_AVEngineContext);
}

bool FFEngine::createContext()
{
    AVOutputFormat *audio_output = av_guess_format(m_audioDevice.toUtf8().constData(), NULL, NULL);
    AVOutputFormat *video_output = av_guess_format(m_videoDevice.toUtf8().constData(), NULL, NULL);;
    if (!video_output || !audio_output) {
        qDebug() << "Device not found";
        return false;
    }
    m_AVEngineContext = avengine_alloc_context(NULL);
    if (!m_AVEngineContext) {
        qDebug() << "Allocated context is NULL";
        return false;
    }
    AVDictionary *opts = NULL;
    av_dict_set(&opts, "no_window", "1", 0);
    avengine_context_set_audio_output(m_AVEngineContext, audio_output, NULL, 0);
    avengine_context_set_video_output(m_AVEngineContext, video_output, opts, 1);
    avengine_context_set_user_data(m_AVEngineContext, this);
    avengine_context_set_player_callbacks(m_AVEngineContext, &m_staticCallbacks);
    return true;
}

bool FFEngine::open(const QString &media)
{
    if (!createContext()) {
        qDebug() << "Cannot allocate AVEngineContext";
        return false;
    }
    if (avengine_open_media(m_AVEngineContext, media.toUtf8().constData()) < 0) {
        avengine_context_free(&m_AVEngineContext);
        qDebug() << "Failed to open stream" << media;
        return false;
    }
    return true;
}

void FFEngine::togglePause()
{
    avengine_toggle_pause(m_AVEngineContext);
}