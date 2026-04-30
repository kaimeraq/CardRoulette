#pragma once

#include <Game/GameMode.h>
#include <Core/Delegate.h>
#include "Gameplay/CRState.h"
#include "IGameQuery.h"

DELEGATE_ONE_PARAM(BoundOnGameOver, const GameResult&);

class CRMode : public GameMode
{
    CRState& GetCRState()
    {
        return static_cast<CRState&>(*m_state);
    }

public:
    BoundOnGameOver OnGameOver;

    CRMode(int playerAmt)
        : GameMode(playerAmt, std::make_unique<CRState>())
    {}

	void BeginPlay() override;
};