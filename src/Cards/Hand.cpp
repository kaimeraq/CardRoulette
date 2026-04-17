#include "Hand.h"

#include <cassert>
#include <print>

void Hand::AddCard(CardInstance instance)
{
    if (!instance.GetCard())
    {
        return; // TODO: log something here
    }

    assert(m_cardCount < MAX_NUM_CARDS_HAND);
    m_cards[m_cardCount++] = instance;
}

void Hand::AddCards(std::initializer_list<CardInstance> cards)
{
    for (auto instance : cards)
    {
        AddCard(instance);
    }
}

int Hand::SumHand() const
{
    int sum = 0;

    for (int i = 0; i < m_cardCount; i++)
    {
        sum += Card::GetValue(m_cards[i].GetCard()->face) * m_cards[i].attrib.multiplierValue;
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