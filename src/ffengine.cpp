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
#include <QSize>
#include <QRect>
#include "ffengine.h"
#include "playermanager.h"
#include "videowidget.h"

AVEngineCallback FFEngine::m_staticCallbacks;

int FFEngine::initializeFFmpeg()
{
    m_staticCallbacks.finished = staticFinishedCallback;
    m_staticCallbacks.pause_state_changed = staticPauseChangedCallback;
    m_staticCallbacks.position_changed = staticPositionChanged;
    m_staticCallbacks.duration_changed = staticDurationChanged;
    m_staticCallbacks.audio_output_context_created = staticAudioOutputContextCreatedCallback;
    m_staticCallbacks.video_output_context_created = staticVideoOutputContextCreatedCallback;

    av_register_all();
    avdevice_register_all();
    avfilter_register_all();
    avformat_network_init();
    if (avengine_init(SystemDelegates::qtSystemDelegates) < 0)
        return 0;
    av_log_set_level(AV_LOG_DEBUG);
    return 1;
}

FFEngine::FFEngine(const QString &videoDevice, const QString &audioDevice,
                   const DeviceOptions &videoOptions, const DeviceOptions &audioOptions,
                   QObject *parent) :
    QObject(parent),
    m_AVEngineContext(NULL),
    m_videoDevice(videoDevice),
    m_audioDevice(audioDevice),
    m_videoOptions(videoOptions),
    m_audioOptions(audioOptions)
{
    static int init = initializeFFmpeg();
    Q_UNUSED(init)
}

FFEngine::~FFEngine()
{
    freeContext();
}

int FFEngine::staticControlMessage(struct AVFormatContext *ctx, int type,
                                   void *data, size_t size)
{
    Q_UNUSED(data); Q_UNUSED(size);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_opaque(ctx));
    switch(static_cast<AVDevToAppMessageType>(type)) {
    case AV_DEV_TO_APP_PREPARE_WINDOW_BUFFER:
        _this->prepareBuffer();
        break;
    case AV_DEV_TO_APP_DISPLAY_WINDOW_BUFFER:
        _this->swapBuffer();
        break;
    case AV_DEV_TO_APP_MUTE_STATE_CHANGED:
        _this->muteChanged(*static_cast<int *>(data));
        break;
    case AV_DEV_TO_APP_VOLUME_LEVEL_CHANGED:
        _this->volumeChanged(*static_cast<double *>(data));
        break;
    default:
        break;
    }
    return 0;
}

void FFEngine::staticFinishedCallback(AVEngineContext *ctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->finishedCallback();
}

void FFEngine::staticPauseChangedCallback(AVEngineContext *ctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->pauseChangedCallback();
}

void FFEngine::staticPositionChanged(AVEngineContext *ctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->positionChanged();
}

void FFEngine::staticDurationChanged(AVEngineContext *ctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->durationChanged();
}

void FFEngine::staticAudioOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *actx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->audioOutputContextCreatedCallback(actx);
}

void FFEngine::staticVideoOutputContextCreatedCallback(AVEngineContext *ctx, AVFormatContext *vctx)
{
    FFEngine *_this = static_cast<FFEngine *>(avengine_context_get_user_data(ctx));
    _this->videoOutputContextCreatedCallback(vctx);
}

void FFEngine::finishedCallback()
{
    emit finished();
}

void FFEngine::pauseChangedCallback()
{
    int paused = avengine_is_paused(m_AVEngineContext);
    if (paused > 0)
        emit FFEngine::paused();
    else if (!paused)
        emit FFEngine::resumed();
}

void FFEngine::positionChanged()
{
    double position = avengine_get_position(m_AVEngineContext);
    //qDebug() << "position" << position;
    if (!isnan(position))
        emit positionChanged(position);
}

void FFEngine::durationChanged()
{
    double duration = avengine_get_duration(m_AVEngineContext);
    //qDebug() << "duration" << duration;
    if (!isnan(duration))
        emit durationChanged(duration);
}

void FFEngine::audioOutputContextCreatedCallback(AVFormatContext *actx)
{
    av_format_set_opaque(actx, this);
    av_format_set_control_message_cb(actx, staticControlMessage);
}

void FFEngine::videoOutputContextCreatedCallback(AVFormatContext *vctx)
{
    av_format_set_opaque(vctx, this);
    av_format_set_control_message_cb(vctx, staticControlMessage);
    AVDeviceRect message;
    message.x = message.y = 0;
    message.height = PlayerManager::instance().getWindow(this)->windowHeight();
    message.width = PlayerManager::instance().getWindow(this)->windowWidth();
    avdevice_app_to_dev_control_message(vctx, AV_APP_TO_DEV_WINDOW_SIZE, &message, sizeof(AVDeviceRect));
}

void FFEngine::freeContext()
{
    avengine_close_media(m_AVEngineContext);
    avengine_context_free(&m_AVEngineContext);
}

bool FFEngine::createContext()
{
    AVOutputFormat *audio_output = av_guess_format(m_audioDevice.toUtf8().constData(), NULL, NULL);
    AVOutputFormat *video_output = av_guess_format(m_videoDevice.toUtf8().constData(), NULL, NULL);
    if (!video_output || !audio_output) {
        qDebug() << "Device not found";
        return false;
    }
    m_AVEngineContext = avengine_alloc_context(NULL);
    if (!m_AVEngineContext) {
        qDebug() << "Allocated context is NULL";
        return false;
    }

    DeviceOptions::const_iterator it;
    AVDictionary *opts = NULL;
    for (it = m_audioOptions.begin(); it != m_audioOptions.end(); ++it)
        av_dict_set(&opts, it.key().toUtf8().constData(), it.value().toUtf8().constData(), 0);
    avengine_context_set_audio_output(m_AVEngineContext, audio_output, opts, 1);
    opts = NULL;
    for (it = m_videoOptions.begin(); it != m_videoOptions.end(); ++it)
        av_dict_set(&opts, it.key().toUtf8().constData(), it.value().toUtf8().constData(), 0);
    avengine_context_set_video_output(m_AVEngineContext, video_output, opts, 1);
    avengine_context_set_user_data(m_AVEngineContext, this);
    avengine_context_set_player_callbacks(m_AVEngineContext, &m_staticCallbacks);
    return true;
}

bool FFEngine::openMedia(const QString &media)
{
    if (m_AVEngineContext)
        freeContext();
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

bool FFEngine::isMediaOpened() const
{
    if (m_AVEngineContext) {
        return avengine_is_media_opened(m_AVEngineContext);
    }
    return false;
}

bool FFEngine::addGuiDelegate(GuiDelegate *guiDelegate)
{
    QObject *qo = dynamic_cast<QObject *>(guiDelegate);
    if (!qo) {
        qCritical("guiDelegate is not QObject");
        return false;
    }

    QObject::connect(this, SIGNAL(volumeChanged(double)),   qo, SLOT(volumeChanged(double)));
    QObject::connect(this, SIGNAL(muteChanged(int)),        qo, SLOT(muteChanged(int)));
    QObject::connect(this, SIGNAL(durationChanged(double)), qo, SLOT(durationChanged(double)));
    QObject::connect(this, SIGNAL(positionChanged(double)), qo, SLOT(positionChanged(double)));
    QObject::connect(this, SIGNAL(paused()),                qo, SLOT(paused()));
    QObject::connect(this, SIGNAL(resumed()),               qo, SLOT(resumed()));

    return true;
}

void FFEngine::resize(const QSize &size)
{
    if (m_AVEngineContext)
        avengine_resize(m_AVEngineContext, size.width(), size.height());
}

void FFEngine::repaint(const QRect &area)
{
    if (m_AVEngineContext)
        avengine_repaint(m_AVEngineContext, area.x(), area.y(), area.width(), area.height());
}

void FFEngine::togglePause()
{
    if (m_AVEngineContext)
        avengine_toggle_pause(m_AVEngineContext);
}

void FFEngine::pause()
{
    if (m_AVEngineContext)
        avengine_pause(m_AVEngineContext, 1);
}

void FFEngine::resume()
{
    if (m_AVEngineContext)
        avengine_pause(m_AVEngineContext, 0);
}

void FFEngine::seek(double seconds)
{
    if (m_AVEngineContext)
        avengine_seek(m_AVEngineContext, seconds);
}

void FFEngine::setVolume(double volume)
{
    if (m_AVEngineContext)
        avengine_set_volume(m_AVEngineContext, volume);
}

void FFEngine::toggleMute()
{
    if (m_AVEngineContext)
        avengine_toggle_mute(m_AVEngineContext);
}
