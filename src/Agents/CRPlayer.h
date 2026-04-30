#pragma once

#include <Players/Player.h>
#include <Cards/Hand.h>

class CRPlayer : public Player
{
    Hand m_hand;

public:
    using Player::Player;

    void AddCard(CardHandle card);
    int GetHandValue() const;
    const Hand& GetHand() const;
};