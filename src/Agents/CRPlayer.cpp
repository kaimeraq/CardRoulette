#include "CRPlayer.h"

void CRPlayer::AddCard(CardHandle card)
{
    m_hand.AddCard(card);
}

int CRPlayer::GetHandValue() const
{
    return m_hand.SumHand();
}

const Hand& CRPlayer::GetHand() const
{
    return m_hand;
}