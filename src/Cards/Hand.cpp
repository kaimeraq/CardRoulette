#include "Hand.h"

#include <cassert>
#include <print>

void Hand::AddCard(CardHandle handle)
{
    if (!handle.IsValid() || !handle.GetCard())
    {
        return; // TODO: log something here
    }

    assert(m_cardCount < MAX_NUM_CARDS_HAND);

    m_cards[m_cardCount++] = handle;
}

void Hand::AddCards(std::initializer_list<CardHandle> cards)
{
    for (CardHandle handle : cards)
    {
        AddCard(handle);
    }
}

const CardHandle& Hand::GetCard(int index) const
{
    assert(index >= 0 && index < m_cardCount);
    return m_cards[index];
}

int Hand::SumHand() const
{
    int sum = 0;

    for (int i = 0; i < m_cardCount; i++)
    {
        sum += Card::GetValue(m_cards[i].GetCard()->face) * m_cards[i].GetAttrib().multiplierValue;
    }

    return sum;
}

void Hand::LogHand() const
{
    for (int i = 0; i < m_cardCount; i++)
    {
        m_cards[i].GetCard()->Print(false);
        std::println();
    }
}