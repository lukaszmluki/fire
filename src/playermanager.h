/*
 * File:   playermanager.h
 * Author: lmr
 *
 * Created on December 12, 2013, 12:09 PM
 */

#ifndef SRC_PLAYER_MANAGER_H
#define	SRC_PLAYER_MANAGER_H

#include <QMap>

class OpenGLDelegate;
class FFEngine;

class PlayerManager
{
    Q_DISABLE_COPY(PlayerManager)

    PlayerManager();
    virtual ~PlayerManager();
public:
    static PlayerManager& instance();

    FFEngine* registerPlayer(OpenGLDelegate *window);
    bool unregisterPlayer(OpenGLDelegate *window);
    bool unregisterPlayer(FFEngine *player);

    OpenGLDelegate* getWindow(FFEngine *player);
    FFEngine* getPlayer(OpenGLDelegate *window);

private:
    typedef QMap<OpenGLDelegate *, FFEngine *> PlayersList;
    typedef PlayersList::iterator PlayersIterator;
    PlayersList m_players;
};

#endif	/* SRC_PLAYER_MANAGER_H */
