#include "CRState.h"
#include "Agents/CRPlayer.h"

#include <cassert>

bool CRState::AddPlayer(int index)
{
    assert(index >= 0 && index < MAX_NUM_PLAYERS);

    if (m_playerOccupied[index])
    {
        return false;
    }

    m_players[index] = std::make_unique<CRPlayer>(index);
    m_playerOccupied[index] = true;

    return true;
}

void CRState::DealCardToPlayer(int index)
{
    assert(index >= 0 && index < MAX_NUM_PLAYERS);
    static_cast<CRPlayer&>(*m_players[index]).AddCard(m_deck.Draw());
}

void CRState::ShuffleDeck()
{
    m_deck.ShuffleDeck();
}

const Hand& CRState::GetPlayerHand(int index) const
{
    assert(index >= 0 && index < MAX_NUM_PLAYERS);
    return static_cast<const CRPlayer&>(GetPlayer(index)).GetHand();
}