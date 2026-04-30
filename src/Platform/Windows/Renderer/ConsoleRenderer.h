#pragma once

#include "UI/CardDisplay.h"

#include <iostream>
#include <format>

class ConsoleRenderer : public Renderer, 
                        public CardDisplayComp, 
                        public HandDisplayComp,
                        public DeckDisplayComp
{
protected:
    void OnDisplayCard(const Card& card) override;
    void OnDisplayHand(const Hand& hand) override;
    void OnDisplayDeck(const Deck& deck, bool bOnSingleRow = false) override;
    void Clear() override;

public:
    ConsoleRenderer() = default;
    ~ConsoleRenderer() = default;

    // Temporary for convenience in widgets
    static ConsoleRenderer& Get() {
        static ConsoleRenderer instance;
        return instance;
    }

    ConsoleRenderer(const ConsoleRenderer&) = delete;
    ConsoleRenderer& operator=(const ConsoleRenderer&) = delete;

    void Print(const GENSTRING& text);
    void Println(const GENSTRING& text);

    template<typename... Args>
    void Print(GENSTRINGFMT<Args...> fmt, Args&&... args)
    {
        GENSTRING buffer;
        buffer.reserve(256);

        std::format_to(std::back_inserter(buffer), fmt, std::forward<Args>(args)...);

        Print(buffer);
    }

    template<typename... Args>
    void Println(GENSTRINGFMT<Args...> fmt, Args&&... args)
    {
        GENSTRING buffer;
        buffer.reserve(256);

        std::format_to(std::back_inserter(buffer), fmt, std::forward<Args>(args)...);
        buffer += GENTEXT('\n');

        Print(buffer);
    }

    void ForceClear();

private:
    int GetCardOffset(const Card& card) const;

    void StageCardRow(GENSTRING& buff, int cardOffset, int row, bool isSuitColored) const;
    void StageDeckSingleRow(GENSTRING& frame, const Deck& deck);
    void StageDeckChunked(GENSTRING& frame, const Deck& deck);

    void SubmitFrame(GENSTRING frame);
};