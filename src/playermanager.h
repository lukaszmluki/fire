/*
 * File:   playermanager.h
 * Author: lmr
 *
 * Created on December 12, 2013, 12:09 PM
 */

#ifndef SRC_PLAYER_MANAGER_H
#define	SRC_PLAYER_MANAGER_H

#include <QList>

class VideoWidget;
class FFEngine;

class PlayerManager
{
    Q_DISABLE_COPY(PlayerManager)

    PlayerManager();
    virtual ~PlayerManager();

public:

    enum PlayerLocalization {
        MAIN_PLAYER
    };

    struct PlayerDesciption {
        VideoWidget *m_videoWidget;
        FFEngine *m_engine;
        PlayerLocalization m_localization;
    };

    static PlayerManager& instance();

    FFEngine* registerPlayer(VideoWidget *window, PlayerLocalization localization);
    bool unregisterPlayer(VideoWidget *window);

    VideoWidget* getWindow(FFEngine *player);
    FFEngine* getPlayer(PlayerLocalization localization);

protected:
    const struct PlayerDesciption* findDescription(const VideoWidget *videoWidget) const;
    const struct PlayerDesciption* findDescription(const FFEngine *engine) const;
    const struct PlayerDesciption* findDescription(const PlayerLocalization localization) const;

    bool hasPlayer(const VideoWidget *videoWidget) const;
    bool hasPlayer(const FFEngine *engine) const;
    bool hasPlayer(const PlayerLocalization localization) const;

private:
    typedef QList<PlayerDesciption> PlayersList;
    typedef PlayersList::iterator PlayersIterator;
    typedef PlayersList::const_iterator ConstPlayersIterator;
    PlayersList m_players;
};

#endif	/* SRC_PLAYER_MANAGER_H */
