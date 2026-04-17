#include "ConsoleRenderer.h"
#include <Renderer/RenderWorker.h>

#if defined(_WIN32) || defined(_WIN64)
#include <fcntl.h>
#include <io.h>
#include <Windows.h>
#define PLATFORM_WINDOWS
#endif

constexpr int CARD_WIDTH = 9;
constexpr int CARD_HEIGHT = 7;
constexpr int CARD_AREA = CARD_WIDTH * CARD_HEIGHT;

/*namespace
{
    struct VTInit
    {
        VTInit()
        {
#ifdef PLATFORM_WINDOWS
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD mode = 0;
            GetConsoleMode(hOut, &mode);
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
        }
    };

    static VTInit s_vtInit;
}*/

namespace
{
    static const wchar_t* ConvertCharToEncodedWide(unsigned char c)
    {
        switch (c) 
        {
        case 'r': return L"\u250C";
        case 'l': return L"\u2510";
        case 's': return L"\u2514";
        case 'd': return L"\u2518";
        case '-': return L"\u2500";
        case '|': return L"\u2502";
        case '.': return L" ";
        case 'S': return L"\u2660";
        case 'C': return L"\u2663";
        case 'H': return L"\u2665";
        case 'D': return L"\u2666";
        case 'b': return L"\u218A";
        case 'E': return L"\u218B";
        case 'u': return L"\u03DB";
        case 'v': return L"\u02C5";
        case '^': return L"\u02C4";
        case '_': return L"\u203F";
        case '=': return L"\u2040";
        case 'y': return L"\u0550";
        case ',': return L"\u22CF";
        case 't': return L"\u2534";
        case '$': return L"\u02C5";
        case 'O': return L"\u053E";
        case 'T': return L"\u2021";
        case '{': return L"\uFD3E";
        case '}': return L"\uFD3F";
        case 'w': return L"\u02AC";
        case '&': return L"\u04DC";
        case 'k': return L"\uA7B0";
        case 'o': return L"\u2092";
        case 'V': return L"\u15C4";
        case 'A': return L"\u15C5";
        case 'I': return L"\u0196";
        default:
        {
            static wchar_t fallback[2] = {};
            fallback[0] = (wchar_t)c;
            fallback[1] = L'\0';
            return fallback;
        }
        }
    }

    static std::wstring ToWide(const std::string& text)
    {
        if (text.empty())
        {
            return {};
        }

        int size = MultiByteToWideChar(CP_UTF8, 0, text.data(), (int)text.size(), nullptr, 0);
        std::wstring wide(size, 0);

        MultiByteToWideChar(CP_UTF8, 0, text.data(), (int)text.size(), &wide[0], size);
        return wide;
    }

    template<typename... Args>
    static std::wstring ToWide(std::format_string<Args...> fmt, Args&&... args)
    {
        return ToWide(std::format(fmt, std::forward<Args>(args)...));
    }
}

void ConsoleRenderer::SubmitFrame(std::wstring frame)
{
    RenderWorker::Instance().Submit(
        [frame = std::move(frame)]() mutable
        {
#ifdef PLATFORM_WINDOWS
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            DWORD written;
            WriteConsoleW(hOut, frame.c_str(), 
                static_cast<DWORD>(frame.size()), 
                &written, nullptr);
#endif
        });
}

int ConsoleRenderer::GetCardOffset(const Card& card) const
{
    return (card.face.value * Card::NUM_SUITS + card.suit.value) * CARD_AREA;
}

void ConsoleRenderer::RenderCardRow(std::wstring& buff, int cardOffset, int row, bool isColorCard) const
{
    const char* data = (const char*)s_cardsData;
    const unsigned char* mask = (const unsigned char*)s_cardsData + (Card::DECK_SIZE * CARD_AREA);
    unsigned char colorCode = data[sizeof(s_cardsData) - 1];

    wchar_t ansiColor[16]{};
    wchar_t ansiReset[] = L"\x1B[0m";
    swprintf(ansiColor, _countof(ansiColor), L"\x1B[%dm", colorCode);

    int rowOffset = row * CARD_WIDTH;
    
    for (int col = 0; col < CARD_WIDTH; col++)
    {
        unsigned char c = (unsigned char)*(data + cardOffset + rowOffset + col);

        buff += (isColorCard && mask[rowOffset + col]) ? ansiColor : ansiReset;
        buff += ConvertCharToEncodedWide(c);
    }
}

static bool IsColorCard(const Card& card)
{
    return card.suit.value == Card::SuitID::Hearts || card.suit.value == Card::SuitID::Diamonds;
}

void ConsoleRenderer::OnDisplayCard(const Card& card)
{
    int cardOffset = GetCardOffset(card);
    std::wstring frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * 16);

    for (int row = 0; row < CARD_HEIGHT; row++)
    {
        RenderCardRow(frame, cardOffset, row, IsColorCard(card));
        frame += L'\n';
    }

    SubmitFrame(std::move(frame));
}

void ConsoleRenderer::OnDisplayHand(const Hand& hand)
{
    //Clear();
    std::wstring frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * hand.GetCardCount() * 16);

    for (int row = 0; row < CARD_HEIGHT; row++)
    {
        for (int index = 0; index < hand.GetCardCount(); index++)
        {
            const Card* card = hand.GetCards()[index].GetCard();
            RenderCardRow(frame, GetCardOffset(*card), row, IsColorCard(*card));
        }

        frame += L'\n';
    }

    SubmitFrame(std::move(frame));
}

void ConsoleRenderer::OnDisplayDeck(const Deck& deck)
{
    Clear();
    std::wstring frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * Card::DECK_SIZE * 16);

    for (int chunk = 0; chunk < Card::DECK_SIZE; chunk += Card::NUM_FACES)
    {
        int cardsInChunk = min(Card::NUM_FACES, Card::DECK_SIZE - chunk);

        for (int row = 0; row < CARD_HEIGHT; row++)
        {
            for (int index = 0; index < cardsInChunk; index++)
            {
                const Card* card = deck.GetCards()[chunk + index].GetCard();
                RenderCardRow(frame, GetCardOffset(*card), row, IsColorCard(*card));
            }

            frame += L'\n';
        }
    }

    SubmitFrame(std::move(frame));
}

// Credit to user CatPlusPlus
// Link: https://stackoverflow.com/a/6487534
void ConsoleRenderer::Clear()
{
    // Clear the screen
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);

    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, screen.wAttributes, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);

    SetConsoleCursorPosition(console, topLeft);
}

void ConsoleRenderer::ForceClear()
{
    Clear();
}

void ConsoleRenderer::Print(const std::wstring& text)
{
    SubmitFrame(text);
}

void ConsoleRenderer::Println(const std::wstring& text)
{
    Print(text + L'\n');
}

void ConsoleRenderer::Print(const std::string& text)
{
    Print(ToWide(text));
}

void ConsoleRenderer::Println(const std::string& text)
{
    Print(ToWide(text) + L'\n');
}