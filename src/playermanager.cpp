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

FFEngine* PlayerManager::registerPlayer(VideoWidget *window)
{
    if (m_players.contains(window))
        return getPlayer(window);

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

    FFEngine *engine = new FFEngine(videoDevice, audioDevice, videoOptions, audioOptions, qo);

    m_players.insert(window, engine);

    QObject::connect(engine, SIGNAL(prepareBuffer()), qo, SLOT(prepareBuffer()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(swapBuffer()), qo, SLOT(swapBuffer()), Qt::DirectConnection);

    return engine;
}

bool PlayerManager::unregisterPlayer(VideoWidget *window)
{
    delete m_players.value(window, NULL);
    return m_players.remove(window);
}

bool PlayerManager::unregisterPlayer(FFEngine *player)
{
    PlayersIterator it = m_players.begin();
    while (it != m_players.end()) {
        if (it.value() == player) {
            delete player;
            m_players.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

VideoWidget* PlayerManager::getWindow(FFEngine *player)
{
    return m_players.key(player);
}

FFEngine* PlayerManager::getPlayer(VideoWidget *window)
{
    return m_players.value(window, NULL);
}
