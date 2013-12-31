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

    QObject::connect(engine, SIGNAL(beforeWriteHeader()), qo, SLOT(moveContextToDeviceThread()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(beforeWriteHeader()), qo, SLOT(makeContextCurrent()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(beforeWritePacket()), qo, SLOT(makeContextCurrent()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(beforeWriteTrailer()), qo, SLOT(makeContextCurrent()), Qt::DirectConnection);

    QObject::connect(engine, SIGNAL(afterWriteHeader()), qo, SLOT(swapBuffer()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(afterWritePacket()), qo, SLOT(swapBuffer()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(afterWriteTrailer()), qo, SLOT(swapBuffer()), Qt::DirectConnection);
    QObject::connect(engine, SIGNAL(afterWriteTrailer()), qo, SLOT(moveContextToMainThread()), Qt::DirectConnection);

    QObject::connect(engine, SIGNAL(getWindowSize(int *, int *)), qo, SLOT(getWindowSize(int *, int *)), Qt::DirectConnection);

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
