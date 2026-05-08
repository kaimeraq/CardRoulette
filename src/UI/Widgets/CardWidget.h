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
            auto* comp = Renderer::Get().As<CardDisplayComp>();

            if (comp)
            {
                comp->DisplayCard(*m_card);
            }
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
            auto* comp = Renderer::Get().As<HandDisplayComp>();

            if (comp)
            {
                comp->DisplayHand(*m_hand);
            }
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
            auto* comp = Renderer::Get().As<DeckDisplayComp>();

            if (comp)
            {
                comp->DisplayDeck(*m_deck, m_bOnSingleRow);
            }
        }
    }
};