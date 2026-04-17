#pragma once

#include "Card.h"

#include <array>

class Deck
{
    std::array<CardInstance, Card::DECK_SIZE> m_cards{};
    int m_topIndex = Card::DECK_SIZE - 1;

public:
    Deck();

    void ShuffleDeck();
    
    const CardInstance* Draw();
    void Put(const CardInstance* card);
    
    void LogDeck() const;
    const CardInstance* GetCards() const { return m_cards.data(); }
    int GetCardCount() const { return Card::DECK_SIZE; }
};