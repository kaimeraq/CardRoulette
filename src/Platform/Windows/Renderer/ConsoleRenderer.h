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

    void Print(const std::wstring& text);
    void Print(const std::string& text);
    void Println(const std::string& text);
    void Println(const std::wstring& text);

    template<typename... Args>
    void Print(std::format_string<Args...> fmt, Args&&... args)
    {
        std::wstring buffer;
        buffer.reserve(256);

        std::format_to(std::back_inserter(buffer), fmt, std::forward<Args>(args)...);

        Print(buffer);
    }

    template<typename... Args>
    void Println(std::format_string<Args...> fmt, Args&&... args)
    {
        std::wstring buffer;
        buffer.reserve(256);

        std::format_to(std::back_inserter(buffer), fmt, std::forward<Args>(args)...);
        buffer += L'\n';

        Print(buffer);
    }

    void ForceClear();

private:
    int GetCardOffset(const Card& card) const;

    void StageCardRow(std::wstring& buff, int cardOffset, int row, bool isSuitColored) const;
    void StageDeckSingleRow(std::wstring& frame, const Deck& deck);
    void StageDeckChunked(std::wstring& frame, const Deck& deck);

    void SubmitFrame(std::wstring frame);
};