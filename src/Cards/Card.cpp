#include "Card.h"

#include <print>

namespace
{
    struct LookupTables
    {
        const Card::FaceData* faces[Card::NUM_FACES]{};
        const Card::SuitData* suits[Card::NUM_SUITS]{};

        LookupTables()
        {
            for (const auto& f : Card::s_faces)
            {
                if (f.id >= 0 && f.id < Card::NUM_FACES)
                {
                    faces[f.id] = &f;
                }
            }

            for (const auto& s : Card::s_suits)
            {
                if (s.id >= 0 && s.id < Card::NUM_SUITS)
                {
                    suits[s.id] = &s;
                }
            }
        }
    };

    const LookupTables& GetLookup()
    {
        static LookupTables tables;
        return tables;
    }
}

const Card::FaceData* Card::FindFace(int id)
{
    if (id < 0 || id >= NUM_FACES)
    {
        return nullptr;
    }

    return GetLookup().faces[id];
}

const Card::SuitData* Card::FindSuit(int id)
{
    if (id < 0 || id >= NUM_SUITS)
    {
        return nullptr;
    }

    return GetLookup().suits[id];
}

static const Card* GetDeckPool()
{
    static Card pool[Card::DECK_SIZE];
    static bool initialized = false;

    if (!initialized)
    {
        for (int suit = 0; suit < Card::NUM_SUITS; ++suit)
        {
            for (int face = 0; face < Card::NUM_FACES; ++face)
            {
                pool[suit * Card::NUM_FACES + face] = Card{ Card::FaceIndex{face}, Card::SuitIndex{suit} };
            }

            initialized = true;
        }
    }

    return pool;
}

const Card* Card::FindById(int id)
{
    if (id < 0 || id >= DECK_SIZE) return nullptr;
    return &GetDeckPool()[id];
}

bool Card::IsValid() const
{
    return face.value != FaceID::INVALID && suit.value != SuitID::INVALID;
}

const char* Card::ToString(Card::FaceIndex face)
{
    const FaceData* data = FindFace(face.value);
    return data ? data->name : "INVALID";
}

const char* Card::ToString(Card::SuitIndex suit)
{
    const SuitData* data = FindSuit(suit.value);
    return data ? data->name : "INVALID";
}

int Card::GetValue(Card::FaceIndex face)
{
    const FaceData* data = FindFace(face.value);
    return data ? data->value : 0;
}

void Card::Print(bool bIncludeValue) const
{
    if (!IsValid())
    {
        std::print("INVALID CARD - {} of {}", ToString(face), ToString(suit));
        return;
    }

    if (bIncludeValue)
    {
        std::print("{} of {} with a value of {}", ToString(face), ToString(suit), GetValue(face));
        return;
    }

    std::print("{} of {}", ToString(face), ToString(suit));
}