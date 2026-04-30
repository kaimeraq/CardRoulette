#pragma once

#include "Card.h"

struct CardAttribute
{
    int multiplierValue = 1;
};

class CardHandle
{
    CardInstance* m_instance = nullptr;
    CardAttribute m_attrib;

    explicit CardHandle(CardInstance* instance) : m_instance(instance) {}
    friend class Deck;

public:
    CardHandle() = default;
    bool IsValid() const 
    { 
        return m_instance != nullptr; 
    }

    const Card* GetCard() const 
    { 
        return m_instance ? m_instance->GetCard() : nullptr; 
    }

    CardAttribute& GetAttrib() 
    { 
        return m_attrib;
    }

    const CardAttribute& GetAttrib() const 
    { 
        return m_attrib;
    }
};