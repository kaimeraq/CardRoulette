#pragma once

#include <Renderer/Widget.h>
#include "UI/CardDisplay.h"
#include "Cards/Card.h"
#include "Cards/Hand.h"
#include "Cards/Deck.h"

class CardWidget : public Widget
{
    const Card* m_card = nullptr;

public:
    CardWidget(const Card* card)
        : m_card(card) {}

    void Draw() const override
    {
        if (m_card)
        {
            //static_cast<CardRenderer*>(GetRenderer())->DisplayCard(*m_card);
        }
    }
};

class HandWidget : public Widget
{
    const Hand* m_hand = nullptr;

public:
    HandWidget(const Hand* hand)
        : m_hand(hand) {
    }

    void Draw() const override
    {
        if (m_hand)
        {
            //static_cast<HandRenderer*>(GetRenderer())->DisplayHand(*m_hand);
        }
    }
};

class DeckWidget : public Widget
{
    const Deck* m_deck = nullptr;

public:
    DeckWidget(const Deck* deck)
        : m_deck(deck) {
    }

    void Draw() const override
    {
        if (m_deck)
        {
            //static_cast<DeckRenderer*>(GetRenderer())->DisplayDeck(*m_deck);
        }
    }
};