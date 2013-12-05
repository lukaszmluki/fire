/*
 * File:   ffengine.cpp
 * Author: Lukasz Marek
 *
 * Created on December 5, 2013, 3:42 PM
 */

#include <QtCore/QDebug>
extern "C" {
#include <libavengine/avengine.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
}
#include "ffengine.h"
#include "systemdelegates.h"

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

FFEngine::FFEngine() :
    m_AVEngineContext(NULL)
{
    static int init = initializeFFmpeg();
    Q_UNUSED(init)
    qDebug() << "FFEngine::FFEngine()";
}

FFEngine::~FFEngine()
{
    freeContext();
}

void FFEngine::freeContext()
{
    //avengine_close_media(m_AVEngineContext);
    avengine_context_free(&m_AVEngineContext);
}

bool FFEngine::createContext()
{
    AVOutputFormat *audio_output = av_guess_format("alsa", NULL, NULL);
    AVOutputFormat *video_output = av_guess_format("xv", NULL, NULL);;
    if (!video_output || !audio_output) {
        qDebug() << "Device not found";
        return false;
    }
    m_AVEngineContext = avengine_alloc_context(NULL);
    if (!m_AVEngineContext) {
        qDebug() << "Allocated context is NULL";
        return false;
    }
    //avengine_context_set_player_callbacks(ctx, &player_callbacks);
    avengine_context_set_audio_output(m_AVEngineContext, audio_output, NULL, 0);
    avengine_context_set_video_output(m_AVEngineContext, video_output, NULL, 0);
    return true;
}

bool FFEngine::open(const QString &media)
{
    if (!createContext()) {
        qDebug() << "Cannot allocate AVEngineContext";
        return false;
    }
    if (avengine_open_media(m_AVEngineContext, media.toUtf8().constData()) < 0) {
        //av_log(NULL, AV_LOG_FATAL, "Failed to open stream!\n");
        avengine_context_free(&m_AVEngineContext);
        qDebug() << "Cannot open file" << media;
        return false;
    }
    return true;
}
