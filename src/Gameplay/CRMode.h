#pragma once

#include <Game/GameMode.h>
#include "Gameplay/CRState.h"
#include "IGameQuery.h"

DECLARE_DELEGATE_OneParam(FOnGameOver, const GameResult&);

class CRMode : public GameMode
{
    CRState& GetCRState()
    {
        return static_cast<CRState&>(*m_state);
    }

public:
	FOnGameOver OnGameOver;

    CRMode(int playerAmt)
        : GameMode(playerAmt)
    {
        m_state = std::make_unique<CRState>();
    }

	void BeginPlay() override;
};