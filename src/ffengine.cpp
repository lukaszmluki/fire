/*
 * File:   ffengine.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
}
#include <QDebug>
#include <QSize>
#include <QRect>
#include "ffmpeg/ffavdictionary.h"
#include "ffengine.h"
#include "playermanager.h"
#include "videowidget.h"

bool FFEngine::initializeFFmpeg()
{
    av_register_all();
    avdevice_register_all();
    avfilter_register_all();
    avformat_network_init();
    av_log_set_level(AV_LOG_DEBUG);
    return true;
}

bool FFEngine::createSink(AVFilterContext *filter, int idx)
{
    enum AVMediaType type = avfilter_pad_get_type(filter->output_pads, idx);
    const char *sinkName;
    struct AVFilterContext **newFilterContext;
    //TODO: probe device
    const enum AVSampleFormat sampleFormats[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
    const enum AVPixelFormat pixelFormats[] = { AV_PIX_FMT_YUV420P, AV_PIX_FMT_NONE };

    do {
        switch (type) {
        case AVMEDIA_TYPE_VIDEO:
            sinkName =  "buffersink";
            newFilterContext = &m_buffersinkFilter;
            break;
        case AVMEDIA_TYPE_AUDIO:
            sinkName = "abuffersink";
            newFilterContext = &m_abuffersinkFilter;
            break;
        default:
            qWarning("Stream type not supported");
            return false;
        }
        qDebug() << "create sink" << sinkName;
        if (avfilter_graph_create_filter(newFilterContext,
                                         avfilter_get_by_name(sinkName),
                                         sinkName, NULL, this, m_filterGraph) < 0) {
            qCritical("Unable to create filter");
            return false;
        }

        if (type == AVMEDIA_TYPE_VIDEO) {
            if (av_opt_set_int_list(*newFilterContext, "pix_fmts", pixelFormats,  AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN) < 0) {
                qWarning("Cannot configure a sink");
                break;
            }
        } else {
            if (av_opt_set_int_list(*newFilterContext, "sample_fmts", sampleFormats,  AV_SAMPLE_FMT_NONE, AV_OPT_SEARCH_CHILDREN) < 0) {
                qWarning("Cannot configure a sink");
                break;
            }
        }

        if (avfilter_link(filter, idx, *newFilterContext, 0) < 0) {
            qCritical("Unable to link filter");
            break;
        }
        return true;
    } while (0);

    avfilter_free(*newFilterContext);
    *newFilterContext = NULL;
    return false;
}

bool FFEngine::createFilterGraph()
{
    do {
        if (!(m_filterGraph = avfilter_graph_alloc())) {
            qCritical("Unable to alloc graph graph");
            return false;
        }

        QString filterParams;
        QTextStream(&filterParams) << "filename=" << m_mediaUrl << ":s=a+v";
        if (avfilter_graph_create_filter(&m_movieFilter, avfilter_get_by_name("movie"), "movie",
                                         filterParams.toUtf8().constData(), this, m_filterGraph) < 0) {
            qCritical("Unable to create filter");
            break;
        }

        for (unsigned i = 0; i < m_filterGraph->nb_filters; ++i) {
            AVFilterContext *filter = m_filterGraph->filters[i];
            for (unsigned j = 0; j < filter->nb_outputs; ++j) {
                if (!filter->outputs[j]) {
                    if (!createSink(filter, j))
                        break;
                }
            }
        }

        if (avfilter_graph_config(m_filterGraph, NULL) < 0) {
            qCritical("Failed to configure graph");
            break;
        }
        return true;
    } while (0);

    avfilter_graph_free(&m_filterGraph);

    return false;
}

bool FFEngine::createDevice()
{
    Q_ASSERT(!m_deviceContext);

    if (avformat_alloc_output_context2(&m_deviceContext, NULL, "multidevice", NULL) < 0) {
        qCritical("Cannot allocate context for device");
        return false;
    }

    av_format_set_control_message_cb(m_deviceContext, &staticControlMessage);
    av_format_set_opaque(m_deviceContext, this);

    return true;
}

bool FFEngine::openDevice()
{
    Q_ASSERT(m_deviceContext);

    FFAVDictionary opts;
    opts.insert("audio_device", m_audioDeviceName);
    opts.insert("video_device", m_videoDeviceName);
    opts.insert(m_audioDeviceOptions);
    opts.insert(m_videoDeviceOptions);

    qDebug() << opts;

    if (avformat_write_header(m_deviceContext, opts.get()) < 0) {
        qCritical("avformat_write_header failed");
        return false;
    }

    QSize size(PlayerManager::instance().getWindow(this)->windowHeight(),
               PlayerManager::instance().getWindow(this)->windowWidth());
    resize(size);

    return true;
}

bool FFEngine::probeDevice()
{
    return true;
}

bool FFEngine::createStream()
{
    Q_ASSERT(m_deviceContext);
    int streamIndex = 0;

    if (m_buffersinkFilter) {
        Q_ASSERT(m_buffersinkFilter->nb_inputs == 1);
        Q_ASSERT(m_buffersinkFilter->inputs[0]->type == AVMEDIA_TYPE_VIDEO);

        AVFilterLink *link = m_buffersinkFilter->inputs[0];
        AVStream *stream;

        if (!(stream = avformat_new_stream(m_deviceContext, NULL))) {
            qCritical("Failed to create output stream");
            return false;
        }
        stream->codec->codec_type          = link->type;
        stream->time_base                  =
        stream->codec->time_base           = link->time_base;
        stream->codec->codec_id            = AV_CODEC_ID_RAWVIDEO;
        stream->avg_frame_rate             =
        stream->  r_frame_rate             = av_buffersink_get_frame_rate(m_buffersinkFilter);
        stream->codec->width               = link->w;
        stream->codec->height              = link->h;
        stream->codec->sample_aspect_ratio = link->sample_aspect_ratio;
        stream->codec->pix_fmt             = static_cast<AVPixelFormat>(link->format);
        m_videoStreamIndex = streamIndex++;
    }
    if (m_abuffersinkFilter) {
        Q_ASSERT(m_abuffersinkFilter->nb_inputs == 1);
        Q_ASSERT(m_abuffersinkFilter->inputs[0]->type == AVMEDIA_TYPE_AUDIO);

        AVFilterLink *link = m_abuffersinkFilter->inputs[0];
        AVStream *stream;

        if (!(stream = avformat_new_stream(m_deviceContext, NULL))) {
            qCritical("Failed to create output stream");
            return false;
        }
        stream->codec->codec_type     = link->type;
        stream->time_base             =
        stream->codec->time_base      = link->time_base;
        stream->codec->channel_layout = link->channel_layout;
        stream->codec->channels       = avfilter_link_get_channels(link);
        stream->codec->sample_rate    = link->sample_rate;
        stream->codec->sample_fmt     = static_cast<AVSampleFormat>(link->format);
        stream->codec->codec_id       = av_get_pcm_codec(stream->codec->sample_fmt, -1);
        m_audioStreamIndex = streamIndex++;
    }
    return true;
}

int FFEngine::staticControlMessage(struct AVFormatContext *ctx, int type,
                                   void *data, size_t size)
{
    Q_UNUSED(data); Q_UNUSED(size);
    FFEngine *_this = static_cast<FFEngine *>(av_format_get_opaque(ctx));
//    qDebug() << "Got message" << type
//             << QChar((type >> 24) & 0xFF)
//             << QChar((type >> 16) & 0xFF)
//             << QChar((type >> 8)  & 0xFF)
//             << QChar( type        & 0xFF);
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
    case AV_DEV_TO_APP_REQUEST_PACKET:
        _this->requestPacket(*static_cast<int *>(data));
        break;
    default:
        qDebug() << "Unhandled message" << type
                 << QChar((type >> 24) & 0xFF)
                 << QChar((type >> 16) & 0xFF)
                 << QChar((type >> 8)  & 0xFF)
                 << QChar( type        & 0xFF);
        break;
    }
    return 0;
}

FFEngine::FFEngine(const QString &videoDevice, const QString &audioDevice,
                   const DeviceOptions &videoOptions, const DeviceOptions &audioOptions,
                   QObject *parent) :
    QObject(parent),
    m_deviceContext(NULL),
    m_filterGraph(NULL),
    m_movieFilter(NULL),
    m_buffersinkFilter(NULL),
    m_abuffersinkFilter(NULL),
    m_audioStreamIndex(-1),
    m_videoStreamIndex(-1),
    m_videoDeviceName(videoDevice),
    m_audioDeviceName(audioDevice),
    m_videoDeviceOptions(videoOptions),
    m_audioDeviceOptions(audioOptions)
{
    static bool init = initializeFFmpeg();
    Q_UNUSED(init);
    connect(this, SIGNAL(requestPacket(int)), SLOT(pushPacket(int)), Qt::QueuedConnection);
}

FFEngine::~FFEngine()
{
}

bool FFEngine::openMedia(const QString &media)
{
    Q_ASSERT(!isMediaOpened());
    m_mediaUrl = media;
    try {
        if (!createDevice()) {
            throw(1);
        }
        if (!probeDevice()) {
            throw(1);
        }
        if (!createFilterGraph()) {
            throw(1);
        }
        if (!createStream()) {
            throw(1);
        }
        if (!openDevice()) {
            throw(1);
        }
        return true;
    }
    catch (int i) {
        qCritical("Cannot open media %s", qPrintable(media));
        m_mediaUrl.clear();
    }
    return false;
}

bool FFEngine::isMediaOpened() const
{
    return !m_mediaUrl.isEmpty();
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

void FFEngine::pushPacket(int streamIndex)
{
    Q_ASSERT(m_filterGraph);
    int ret;
    AVFilterContext *sink;

    //qDebug() << streamIndex;

    if (streamIndex == m_audioStreamIndex) {
        //qDebug() << "request audio";
        sink = m_abuffersinkFilter;
    } else if (streamIndex == m_videoStreamIndex) {
        //qDebug() << "request video";
        sink = m_buffersinkFilter;
    } else {
        qWarning("Unknown stream index");
        return;
    }

//    if ((ret = avfilter_graph_request_oldest(m_filterGraph)) < 0) {
//        if (ret == AVERROR_EOF) {
//            qDebug() << "____pushPacket TODO: EOF";
//            return;
//        } else if (ret == AVERROR(EAGAIN)) {
//            qDebug() << "____pushPacket TODO: EAGAIN";
//            return;
//        } else {
//            qDebug() << "____pushPacket TODO: ERROR" << ret;
//            return;
//        }
//    }

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        qCritical("Could not allocate frame");
        qDebug() << "____pushPacket TODO: ERROR";
        return;
    }


    ret = av_buffersink_get_frame_flags(sink, frame, AV_BUFFERSINK_FLAG_NO_REQUEST);
    if (ret == AVERROR(EAGAIN))
        ret = av_buffersink_get_frame_flags(sink, frame, 0);
    if (ret < 0) {
        if (ret == AVERROR_EOF) {
            qDebug() << "____pushPacket av_buffersink_get_frame_flags TODO: EOF";
            return;
        } else if (ret == AVERROR(EAGAIN)) {
            qDebug() << "____pushPacket av_buffersink_get_frame_flags TODO: EAGAIN";
            return;
        } else {
            qDebug() << "____pushPacket av_buffersink_get_frame_flags TODO: ERROR" << ret;
            return;
        }
    }

//    if (frame->pts != AV_NOPTS_VALUE)
//        frame->pts = av_rescale_q(frame->pts,
//                                  sink->inputs[0]->time_base,
//                                  m_deviceContext->streams[streamIndex]->time_base);
    ret = av_interleaved_write_uncoded_frame(m_deviceContext, streamIndex, frame);
    frame = NULL;
    if (ret < 0) {
        if (ret == AVERROR_EOF) {
            qDebug() << "____pushPacket av_interleaved_write_uncoded_frame TODO: EOF";
            return;
        } else if (ret == AVERROR(EAGAIN)) {
            qDebug() << "____pushPacket av_interleaved_write_uncoded_frame TODO: EAGAIN";
            return;
        } else {
            qDebug() << "____pushPacket av_interleaved_write_uncoded_frame TODO: ERROR" << ret;
            return;
        }
    }
}

void FFEngine::resize(const QSize &size)
{
    if (m_deviceContext) {
        AVDeviceRect message;
        message.x = message.y = 0;
        message.width = size.width();
        message.height = size.height();
        avdevice_app_to_dev_control_message(m_deviceContext, AV_APP_TO_DEV_WINDOW_SIZE,
                                            &message, sizeof(AVDeviceRect));
    }
}

void FFEngine::repaint(const QRect &area)
{
    if (m_deviceContext) {
        AVDeviceRect message;
        message.x = area.x();
        message.y = area.y();
        message.width = area.width();
        message.height = area.height();
        avdevice_app_to_dev_control_message(m_deviceContext, AV_APP_TO_DEV_WINDOW_REPAINT,
                                            &message, sizeof(AVDeviceRect));
    }
}

void FFEngine::togglePause()
{
}

void FFEngine::pause()
{
}

void FFEngine::resume()
{
}

void FFEngine::seek(double seconds)
{
    Q_UNUSED(seconds)
}

void FFEngine::setVolume(double volume)
{
    Q_UNUSED(volume)
}

void FFEngine::toggleMute()
{
}
