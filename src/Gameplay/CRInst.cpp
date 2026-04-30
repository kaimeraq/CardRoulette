#include "CRInst.h"
#include "CRMode.h"
#include <Core/Logger.h>

#include <iostream>
#include <print>

constexpr Category Cat_Game = Category::Gameplay;

void CRInst::OnInit()
{

}

std::unique_ptr<GameMode> CRInst::CreateMode()
{
    auto mode = std::make_unique<CRMode>(m_numPlayers);
    mode->OnGameOver.BindLambda([this](const GameResult& result) 
        {
            OnGameOver(result);
        });

    return mode;
}

bool CRInst::OnTick()
{
    switch (m_state)
    {
    case GameLoopState::SELECT:
    {
        LOG(Cat_Game, "Entering select stage");
        std::print("Number of players? (2-{}): ", MAX_NUM_PLAYERS);
        std::cin >> m_numPlayers; // <- This is where I get num players from input

        if (!std::cin)
        {
            return false;
        }

        if (m_numPlayers >= 2 && m_numPlayers <= MAX_NUM_PLAYERS)
        {
            m_state = GameLoopState::START;
        }

        break;
    }
    case GameLoopState::START:
    {
        m_mode = CreateMode();
        m_mode->BeginPlay();
        break;
    }
    case GameLoopState::RESULTS:
    {
        PrintResult();
        m_state = GameLoopState::REPLAY;
        break;
    }
    case GameLoopState::REPLAY:
    {
        ANSICHAR choice;
        std::print("\nPlay Again? (Y/N): ");
        std::cin >> choice;
        std::println();

        if (choice == 'Y' || choice == 'y')
        {
            m_state = GameLoopState::SELECT;
        }
        else
        {
            return false;
        }

        break;
    }
    }

    return true;
}

void CRInst::OnGameOver(const GameResult& result)
{
    m_result = result;
    m_state = GameLoopState::RESULTS;
}

void CRInst::PrintResult() const
{
    if (m_result.numWinners > 1)
    {
        std::print("It's a tie between players ");

        for (int i = 0; i < m_result.numWinners; i++)
        {
            if (i == m_result.numWinners - 1)
            {
                std::print("{}", m_result.winners[i]);
            }
            else if (i == m_result.numWinners - 2 && m_result.numWinners == 2)
            {
                std::print("{} and ", m_result.winners[i]);
            }
            else if (i == m_result.numWinners - 2)
            {
                std::print("{}, and ", m_result.winners[i]);
            }
            else
            {
                std::print("{}, ", m_result.winners[i]);
            }
        }

        std::println(" with a score of {}", m_result.winningScore);
    }
    else
    {
        std::println("Player {} wins with a score of {}!", m_result.winnerID, m_result.winningScore);
    }

    // Debug print winning hand
#if defined(LOGGING) && LOGGING >= 2
    m_result.query->GetPlayerHand(m_result.winnerID).LogHand();
#endif
}