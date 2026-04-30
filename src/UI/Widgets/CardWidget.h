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

    void Render() const override
    {
        if (m_card)
        {
            ConsoleRenderer::Get().DisplayCard(*m_card);
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

    void Render() const override
    {
        if (m_hand)
        {
            ConsoleRenderer::Get().DisplayHand(*m_hand);
        }
    }
};

class DeckWidget : public Widget
{
    const Deck* m_deck = nullptr;
    bool m_bOnSingleRow = false;

public:
    DeckWidget(const Deck* deck, bool bOnSingleRow = false)
        : m_deck(deck), m_bOnSingleRow(bOnSingleRow) {
    }

    void Render() const override
    {
        if (m_deck)
        {
            ConsoleRenderer::Get().DisplayDeck(*m_deck, m_bOnSingleRow);
        }
    }
};