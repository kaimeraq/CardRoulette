#pragma once

#include <Game/GameState.h>
#include <Cards/Deck.h>
#include "IGameQuery.h"

class CRState : public GameState, public IGameQuery
{
    Deck m_deck;

public:
    CRState() = default;
    ~CRState() = default;
    CRState(const CRState&) = delete;
    CRState& operator=(const CRState&) = delete;

    bool AddPlayer(int index) override;
    void DealCardToPlayer(int index);
    void ShuffleDeck();

    const Hand& GetPlayerHand(int index) const override;
};