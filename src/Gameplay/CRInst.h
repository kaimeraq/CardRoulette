#pragma once

#include <Game/GameInstance.h>
#include "IGameQuery.h"
#include "Platform/Windows/Renderer/ConsoleRenderer.h"

class CRInst : public GameInstance
{
	GameLoopState m_state = GameLoopState::SELECT;
	GameResult m_result;

	void PrintResult() const;
	void OnGameOver(const GameResult& result);

protected:
	void OnInit() override;
	bool OnTick() override;
	std::unique_ptr<GameMode> CreateMode() override;

public:
	CRInst()
		: GameInstance(std::make_unique<ConsoleRenderer>())
	{}
};