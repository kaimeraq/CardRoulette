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
    const IGameQuery* query{};
    std::array<int, MAX_NUM_PLAYERS> winners{};
    int winnerID = -1;
    int winningScore = 0;
    int numWinners = 0;
};