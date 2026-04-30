#include "Deck.h"

#include "Core/Logger.h"

#include <cassert>
#include <print>
#include <random>

static std::mt19937 s_rng{ std::random_device{}() };

CardInstance Deck::s_canonicalDeck[Card::DECK_SIZE] = {};
bool Deck::s_initialized = false;

void Deck::Init()
{
    if (s_initialized)
    {
        return;
    }

    for (int i = 0; i < Card::DECK_SIZE; ++i)
    {
        s_canonicalDeck[i] = CardInstance{ i };
    }

    s_initialized = true;
}

Deck::Deck()
{
    Init();

    for (int i = 0; i < Card::DECK_SIZE; ++i)
    {
        m_cards[i] = static_cast<uint8_t>(i);
    }
}

void Deck::ShuffleDeck()
{
    std::shuffle(std::begin(m_cards), std::end(m_cards), s_rng);

#if defined(LOGGING) && LOGGING >= 2
    LogDeck();
#endif
}

CardHandle Deck::Draw()
{
    assert(m_topIndex >= 0);
    return CardHandle(&s_canonicalDeck[m_cards[m_topIndex--]]);
}

void Deck::Put(CardHandle handle)
{
    assert(m_topIndex < Card::DECK_SIZE - 1);

    int index = static_cast<int>(handle.m_instance - s_canonicalDeck);
    m_cards[++m_topIndex] = static_cast<uint8_t>(index);
}

CardHandle Deck::GetCard(int index) const
{
    assert(index >= 0 && index < Card::DECK_SIZE);
    return CardHandle(&s_canonicalDeck[m_cards[index]]);
}

void Deck::LogDeck() const
{
    for (int i = 0; i < Card::DECK_SIZE; ++i)
    {
        s_canonicalDeck[m_cards[i]].GetCard()->Print(false);
        std::println();
    }
}