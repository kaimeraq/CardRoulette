#pragma once

#include <Platform/Platform.h>
#include <iterator>

struct Card
{
    struct FaceID
    {
        static constexpr int INVALID = -1;
        static constexpr int Two = 0;
        static constexpr int Three = 1;
        static constexpr int Four = 2;
        static constexpr int Five = 3;
        static constexpr int Six = 4;
        static constexpr int Seven = 5;
        static constexpr int Eight = 6;
        static constexpr int Nine = 7;
        static constexpr int Ten = 8;
        static constexpr int Jack = 9;
        static constexpr int Queen = 10;
        static constexpr int King = 11;
        static constexpr int Ace = 12;
    };

    struct SuitID
    {
        static constexpr int INVALID = -1;
        static constexpr int Spades = 0;
        static constexpr int Clubs = 1;
        static constexpr int Hearts = 2;
        static constexpr int Diamonds = 3;
    };

    struct FaceData
    {
        ANSICSTR name;
        int id;
        int value;
    };

    struct SuitData
    {
        ANSICSTR name;
        int id;
    };

    struct FaceIndex { int value = FaceID::INVALID; };
    struct SuitIndex { int value = SuitID::INVALID; };

    static constexpr FaceData s_faces[] =
    {
        {.name = "Two",   .id = FaceID::Two,   .value = 2  },
        {.name = "Three", .id = FaceID::Three, .value = 3  },
        {.name = "Four",  .id = FaceID::Four,  .value = 4  },
        {.name = "Five",  .id = FaceID::Five,  .value = 5  },
        {.name = "Six",   .id = FaceID::Six,   .value = 6  },
        {.name = "Seven", .id = FaceID::Seven, .value = 7  },
        {.name = "Eight", .id = FaceID::Eight, .value = 8  },
        {.name = "Nine",  .id = FaceID::Nine,  .value = 9  },
        {.name = "Ten",   .id = FaceID::Ten,   .value = 10 },
        {.name = "Jack",  .id = FaceID::Jack,  .value = 10 },
        {.name = "Queen", .id = FaceID::Queen, .value = 10 },
        {.name = "King",  .id = FaceID::King,  .value = 10 },
        {.name = "Ace",   .id = FaceID::Ace,   .value = 11 },
    };

    static constexpr SuitData s_suits[] =
    {
        {.name = "Spades",   .id = SuitID::Spades,   },
        {.name = "Clubs",    .id = SuitID::Clubs,    },
        {.name = "Hearts",   .id = SuitID::Hearts,   },
        {.name = "Diamonds", .id = SuitID::Diamonds, },
    };

    static constexpr int NUM_FACES = static_cast<int>(std::size(s_faces));
    static constexpr int NUM_SUITS = static_cast<int>(std::size(s_suits));
    static constexpr int DECK_SIZE = NUM_SUITS * NUM_FACES;

    static const FaceData* FindFace(int id);
    static const SuitData* FindSuit(int id);

    FaceIndex face;
    SuitIndex suit;

    Card() = default;
    Card(FaceIndex p_face, SuitIndex p_suit) : face(p_face), suit(p_suit) {}

    static const Card* FindById(int id);
    static int MakeId(FaceIndex face, SuitIndex suit)
    {
        return suit.value * NUM_FACES + face.value;
    }

    bool IsValid() const;

    static ANSICSTR ToString(FaceIndex face);
    static ANSICSTR ToString(SuitIndex suit);
    static int GetValue(FaceIndex face);

    void Print(bool bIncludeValue) const;
};

struct CardInstance
{
    int cardId = Card::FaceID::INVALID;

    const Card* GetCard() const 
    { 
        return Card::FindById(cardId); 
    }
};