/*
 * File:   playermanager.cpp
 * Author: lmr
 *
 * Created on December 12, 2013, 12:09 PM
 */

#include <QDebug>
#include <QWidget>
#include "playermanager.h"
#include "ffengine.h"
#include "videowidget.h"
#include "preferences.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

PlayerManager& PlayerManager::instance()
{
    static PlayerManager instance;
    return instance;
}

const PlayerManager::PlayerDesciption* PlayerManager::findDescription(const VideoWidget *videoWidget) const
{
    ConstPlayersIterator it = m_players.begin();
    for (; it != m_players.end(); ++it) {
        if (it->m_videoWidget == videoWidget)
            return &(*it);
    }
    return NULL;
}

const PlayerManager::PlayerDesciption* PlayerManager::findDescription(const FFEngine *engine) const
{
    ConstPlayersIterator it = m_players.begin();
    for (; it != m_players.end(); ++it) {
        if (it->m_engine == engine)
            return &(*it);
    }
    return NULL;
}

const PlayerManager::PlayerDesciption* PlayerManager::findDescription(const PlayerLocalization localization) const
{
    ConstPlayersIterator it = m_players.begin();
    for (; it != m_players.end(); ++it) {
        if (it->m_localization == localization)
            return &(*it);
    }
    return NULL;
}


bool PlayerManager::hasPlayer(const VideoWidget *videoWidget) const
{
    return findDescription(videoWidget) != NULL;
}

bool PlayerManager::hasPlayer(const FFEngine *engine) const
{
    return findDescription(engine) != NULL;
}

bool PlayerManager::hasPlayer(const PlayerLocalization localization) const
{
    return findDescription(localization) != NULL;
}

FFEngine* PlayerManager::registerPlayer(VideoWidget *window, PlayerLocalization localization)
{
    if (hasPlayer(window))
        return NULL;

    QObject *qo = dynamic_cast<QObject *>(window);
    if (!qo) {
        qCritical("Window is not QObject");
        return NULL;
    }

    QString videoDevice;
    QString audioDevice("pulse");
    FFEngine::DeviceOptions videoOptions;
    FFEngine::DeviceOptions audioOptions;
    Preferences::RenderingEngine renderingEngine = Preferences::instance().getRenderingEngine();
    if (renderingEngine == Preferences::RENDERING_ENGINE_OPENGL) {
        videoOptions["no_window"] = "1";
        videoDevice = "opengl";
    } else if (renderingEngine == Preferences::RENDERING_ENGINE_X11) {
        videoOptions["window_id"] = QString::number(dynamic_cast<QWidget *>(window)->winId());
        videoDevice = "xv";
    } else {
        qCritical() << "Not supported engine";
        return NULL;
    }

    PlayerDesciption desc;
    desc.m_engine = new FFEngine(videoDevice, audioDevice, videoOptions, audioOptions, qo);
    desc.m_videoWidget = window;
    desc.m_localization = localization;

    m_players.prepend(desc);

    QObject::connect(desc.m_engine, SIGNAL(prepareBuffer()), qo, SLOT(prepareBuffer()), Qt::DirectConnection);
    QObject::connect(desc.m_engine, SIGNAL(swapBuffer()), qo, SLOT(swapBuffer()), Qt::DirectConnection);

    return desc.m_engine;
}

bool PlayerManager::unregisterPlayer(VideoWidget *window)
{
    PlayersIterator it = m_players.begin();
    for (; it != m_players.end(); ++it) {
        if (it->m_videoWidget == window) {
            delete it->m_engine;
            m_players.erase(it);
            return true;
        }
    }
    return false;
}

VideoWidget* PlayerManager::getWindow(FFEngine *player)
{
    const PlayerDesciption *desc = findDescription(player);
    if (!desc)
        return NULL;
    return desc->m_videoWidget;
}

FFEngine* PlayerManager::getPlayer(PlayerLocalization localization)
{
    const PlayerDesciption *desc = findDescription(localization);
    if (!desc)
        return NULL;
    return desc->m_engine;
}
