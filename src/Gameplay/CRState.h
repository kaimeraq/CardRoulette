#pragma once

#include <Game/GameState.h>
#include <Cards/Deck.h>
#include "IGameQuery.h"

class CRState : public GameState, public IGameQuery
{
    Deck m_deck;

public:
    bool AddPlayer(int index) override;
    void DealCardToPlayer(int index);
    void ShuffleDeck();

    const Hand& GetPlayerHand(int index) const override;
};