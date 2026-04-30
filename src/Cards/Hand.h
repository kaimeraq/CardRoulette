#pragma once

#include "Card.h"
#include "CardHandle.h"
#include <initializer_list>

constexpr int MAX_NUM_CARDS_HAND = 5;

class Hand
{
    CardHandle m_cards[MAX_NUM_CARDS_HAND] = {};
    int m_cardCount = 0;

public:
    void AddCard(CardHandle handle);
    void AddCards(std::initializer_list<CardHandle> cards);

    const CardHandle& GetCard(int index) const;

    int SumHand() const;
    void LogHand() const;

    int GetCardCount() const 
    { 
        return m_cardCount; 
    }
};