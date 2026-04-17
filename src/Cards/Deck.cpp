#include "Deck.h"

#include "Core/Config.h"

#include <cassert>
#include <print>
#include <random>

static std::mt19937 s_rng{ std::random_device{}() };

Deck::Deck()
{
    for (int i = 0; i < Card::DECK_SIZE; ++i)
    {
        m_cards[i] = CardInstance{ i };
    }
}

void Deck::ShuffleDeck()
{
    std::shuffle(m_cards.begin(), m_cards.end(), s_rng);

#if defined(LOGGING_VERBOSE) && LOGGING_VERBOSE
    LogDeck();
#endif
}

const CardInstance* Deck::Draw()
{
    assert(m_topIndex >= 0);
    return &m_cards[m_topIndex--];
}

void Deck::Put(const CardInstance* card)
{
    if (!card)
    {
        return; // TODO: log something here
    }

    assert(m_topIndex < Card::DECK_SIZE - 1);
    m_cards[++m_topIndex] = *card;
}

void Deck::LogDeck() const
{
    for (const auto& card : m_cards)
    {
        card.GetCard()->Print(false);
        std::println();
    }
}