#pragma once

#include <Game/GameMode.h>
#include <Core/Delegate.h>
#include "Gameplay/CRState.h"
#include "IGameQuery.h"

DELEGATE_ONE_PARAM(BoundOnGameOver, const GameResult&);

class CRMode : public GameMode
{
public:
    BoundOnGameOver OnGameOver;

    CRMode(int playerAmt)
        : GameMode(playerAmt, std::make_unique<CRState>())
    {}

    CRState& GetCRState()
    {
        return static_cast<CRState&>(*m_state);
    }

	void BeginPlay() override;
};