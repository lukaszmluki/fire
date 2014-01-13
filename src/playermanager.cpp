/*
 * File:   playermanager.cpp
 * Author: lmr
 *
 * Created on December 12, 2013, 12:09 PM
 */

#include "playermanager.h"
#include "ffengine.h"
#include "opengldelegate.h"

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

FFEngine* PlayerManager::registerPlayer(OpenGLDelegate *window)
{
    if (m_players.contains(window))
        return getPlayer(window);

    QObject *qo = dynamic_cast<QObject *>(window);
    if (!qo) {
        qCritical("Window is not QObject");
        return NULL;
    }

    FFEngine *engine = new FFEngine("opengl", "pulse", qo);

    m_players.insert(window, engine);

    QObject::connect(engine, SIGNAL(prepareBuffer()), qo, SLOT(prepareBuffer()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(swapBuffer()), qo, SLOT(swapBuffer()), Qt::DirectConnection);

    return engine;
}

bool PlayerManager::unregisterPlayer(OpenGLDelegate *window)
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

OpenGLDelegate* PlayerManager::getWindow(FFEngine *player)
{
    return m_players.key(player);
}

FFEngine* PlayerManager::getPlayer(OpenGLDelegate *window)
{
    return m_players.value(window, NULL);
}
