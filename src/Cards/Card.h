#pragma once

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
        int id;
        const char* name;
        int value;
    };

    struct SuitData
    {
        int id;
        const char* name;
    };

    struct FaceIndex { int value = FaceID::INVALID; };
    struct SuitIndex { int value = SuitID::INVALID; };

    static constexpr FaceData s_faces[] =
    {
        { FaceID::Two,   "Two",   2  },
        { FaceID::Three, "Three", 3  },
        { FaceID::Four,  "Four",  4  },
        { FaceID::Five,  "Five",  5  },
        { FaceID::Six,   "Six",   6  },
        { FaceID::Seven, "Seven", 7  },
        { FaceID::Eight, "Eight", 8  },
        { FaceID::Nine,  "Nine",  9  },
        { FaceID::Ten,   "Ten",   10 },
        { FaceID::Jack,  "Jack",  10 },
        { FaceID::Queen, "Queen", 10 },
        { FaceID::King,  "King",  10 },
        { FaceID::Ace,   "Ace",   11 },
    };

    static constexpr SuitData s_suits[] =
    {
        { SuitID::Spades,   "Spades"   },
        { SuitID::Clubs,    "Clubs"    },
        { SuitID::Hearts,   "Hearts"   },
        { SuitID::Diamonds, "Diamonds" },
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

    static const char* ToString(FaceIndex face);
    static const char* ToString(SuitIndex suit);
    static int GetValue(FaceIndex face);

    void Print(bool bIncludeValue) const;
};

struct CardAttribute
{
    int multiplierValue = 1;
};

struct CardInstance
{
    int cardId = Card::FaceID::INVALID;
    CardAttribute attrib;

    const Card* GetCard() const 
    { 
        return Card::FindById(cardId); 
    }

    int GetValue() const
    {
        return attrib.multiplierValue;
    }
};