#pragma once

#include "Card.h"
#include "CardHandle.h"

class Deck
{
    static CardInstance s_canonicalDeck[Card::DECK_SIZE];
    static bool s_initialized;

    uint8_t m_cards[Card::DECK_SIZE] = {};
    int m_topIndex = Card::DECK_SIZE - 1;

    static void Init();

public:
    Deck();

    void ShuffleDeck();
    
    CardHandle Draw();
    void Put(CardHandle handle);
    CardHandle GetCard(int index) const;
    
    void LogDeck() const;
    
    int GetCardCount() const 
    { 
        return Card::DECK_SIZE; 
    }
};