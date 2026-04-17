#pragma once

#include <Core/CoreMinimal.h>
#include <Cards/Hand.h>

class IGameQuery
{
public:
    virtual const Hand& GetPlayerHand(int index) const = 0;
    virtual ~IGameQuery() = default;
};

enum class GameLoopState
{
    SELECT,
    START,
    PLAYING,
    RESULTS,
    REPLAY
};

struct GameResult
{
    const IGameQuery* query;
    int winnerID;
    int winningScore;
    std::array<int, MAX_NUM_PLAYERS> winners;
    int numWinners = 0;
};