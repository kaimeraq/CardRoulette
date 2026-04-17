#include "CRMode.h"
#include "IGameQuery.h"
#include "Agents/CRPlayer.h"

void CRMode::BeginPlay()
{
    CRState& state = GetCRState();

    for (int i = 0; i < m_numPlayers; i++)
    {
        state.AddPlayer(i);
    }

    state.ShuffleDeck();

    for (int i = 0; i < m_numPlayers; i++)
    {
        for (int j = 0; j < MAX_NUM_CARDS_HAND; j++)
        {
            state.DealCardToPlayer(i);
        }
        
        state.RecordScore(i, static_cast<const CRPlayer&>(state.GetPlayer(i)).GetHandValue());
    }

    // Check for winners
    int winnerID = 0;
    std::array<int, MAX_NUM_PLAYERS> winners{};
    int numWinners = 1;
    winners[0] = 0;

    for (int i = 1; i < m_numPlayers; i++)
    {
        if (state.GetScore(i) > state.GetScore(winnerID))
        {
            winnerID = i;
            numWinners = 1;
            winners[0] = i;

        }
        else if (state.GetScore(i) == state.GetScore(winnerID))
        {
            winners[numWinners++] = i;
        }
    }

    GameResult result{ &state, winnerID, state.GetScore(winnerID), winners, numWinners };
    OnGameOver.ExecuteIfBound(result);
}