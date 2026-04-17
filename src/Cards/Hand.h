#pragma once

#include "Card.h"

#include <array>
#include <initializer_list>

constexpr int MAX_NUM_CARDS_HAND = 5;

class Hand
{
    std::array<CardInstance, MAX_NUM_CARDS_HAND> m_cards{};
    int m_cardCount = 0;

public:
    void AddCard(CardInstance instance);
    void AddCards(std::initializer_list<CardInstance> cards);

    int SumHand() const;
    void LogHand() const;

    const CardInstance* GetCards() const { return m_cards.data(); }
    int GetCardCount() const { return m_cardCount; }
};