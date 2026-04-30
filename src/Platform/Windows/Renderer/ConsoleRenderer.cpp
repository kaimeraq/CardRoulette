#include <Platform/Platform.h>
#include <Core/CoreMinimal.h>
#include <Renderer/RenderWorker.h>

#include "ConsoleRenderer.h"
#include "Assets/CardConfig.h"

constexpr Category Cat_Renderer = Category::Renderer;

constexpr int CARD_WIDTH = 9;
constexpr int CARD_HEIGHT = 7;
constexpr int CARD_AREA = CARD_WIDTH * CARD_HEIGHT;

namespace
{
    struct VTInit
    {
        VTInit()
        {
#ifdef PLATFORM_WINDOWS
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE)
            {
                CRASH_FATAL(Cat_Renderer, "Couldn't get the console output handle.");
            }

            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            if (hIn == INVALID_HANDLE_VALUE)
            {
                CRASH_FATAL(Cat_Renderer, "Couldn't get the console input handle.");
            }

            DWORD dwOriginalOutMode = 0;
            DWORD dwOriginalInMode = 0;

            if (!GetConsoleMode(hOut, &dwOriginalOutMode))
            {
                CRASH_FATAL(Cat_Renderer, "Couldn't get the console output mode.");
            }
            if (!GetConsoleMode(hIn, &dwOriginalInMode))
            {
                CRASH_FATAL(Cat_Renderer, "Couldn't get the console input mode.");
            }

            DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

            DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
            if (!SetConsoleMode(hOut, dwOutMode))
            {
                // we failed to set both modes, try to step down mode gracefully.
                dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                dwOutMode = dwOriginalOutMode | dwRequestedOutModes;

                if (!SetConsoleMode(hOut, dwOutMode))
                {
                    // Failed to set any VT mode, can't do anything here.
                    CRASH_FATAL(Cat_Renderer, "Failed to set any VT output modes.");
                }
            }

            DWORD dwInMode = dwOriginalInMode | dwRequestedInModes;
            if (!SetConsoleMode(hIn, dwInMode))
            {
                // Failed to set VT input mode, can't do anything here.
                CRASH_FATAL(Cat_Renderer, "Failed to set any VT input modes.");
            }

            Logger::Get().bVTSupported = true;
            LOG_VERBOSE(Cat_Renderer, "Successfully set VT modes.");
#endif
        }
    };

    static VTInit s_vtInit;
}

namespace
{
    static GENCSTR ConvertCharToEncoded(UANSICHAR c)
    {
        switch (c)
        {
        case 'r': return GENTEXT("\u250C");
        case 'l': return GENTEXT("\u2510");
        case 's': return GENTEXT("\u2514");
        case 'd': return GENTEXT("\u2518");
        case '-': return GENTEXT("\u2500");
        case '|': return GENTEXT("\u2502");
        case '.': return GENTEXT(" "; )
        case 'S': return GENTEXT("\u2660");
        case 'C': return GENTEXT("\u2663");
        case 'H': return GENTEXT("\u2665");
        case 'D': return GENTEXT("\u2666");
        case 'b': return GENTEXT("\u218A");
        case 'E': return GENTEXT("\u218B");
        case '$': return GENTEXT("\u03DB");
        case 'v': return GENTEXT("\u02C5");
        case '^': return GENTEXT("\u02C4");
        case '_': return GENTEXT("\u203F");
        case '=': return GENTEXT("\u2040");
        case 'y': return GENTEXT("\u0550");
        case ',': return GENTEXT("\u22CF");
        case 't': return GENTEXT("\u2534");
        case 'u': return GENTEXT("\u02C5");
        case 'O': return GENTEXT("\u053E");
        case 'T': return GENTEXT("\u2021");
        case '{': return GENTEXT("\uFD3E");
        case '}': return GENTEXT("\uFD3F");
        case 'w': return GENTEXT("\u02AC");
        case '&': return GENTEXT("\u04DC");
        case 'k': return GENTEXT("\uA7B0");
        case 'o': return GENTEXT("\u2092");
        case 'V': return GENTEXT("\u15C4");
        case 'A': return GENTEXT("\u15C5");
        case 'I': return GENTEXT("\u0196");
        case 'W': return GENTEXT("\uA4E4");
        default:
        {
            static GENCHAR fallback[2] = {};
            fallback[0] = (GENCHAR)c;
            fallback[1] = L'\0';
            return fallback;
        }
        }
    }

    static bool IsValueIdentifier(UANSICHAR c)
    {
        static const UANSICHAR kValues[] =
        {
            '2','3','4','5','6','7','8','9','1','0',
            'J','Q','K','A','b','E','h','u','L','V',
            'y','I','k','O'
        };

        for (UANSICHAR v : kValues)
        {
            if (c == v)
            {
                return true;
            }
        }

        return false;
    }

    static bool IsBorderIdentifier(UANSICHAR c)
    {
        static const UANSICHAR kBorders[] =
        {
            'r','l','s','d','-','|'
        };

        for (UANSICHAR v : kBorders)
        {
            if (c == v)
            {
                return true;
            }
        }

        return false;
    }

    static bool IsExtraIdentifier(UANSICHAR c)
    {
        static const UANSICHAR kExtras[] =
        {
            '\\','/','{','}','_',',','&'
        };

        for (UANSICHAR v : kExtras)
        {
            if (c == v)
            {
                return true;
            }
        }

        return false;
    }

    static bool IsColorable(bool isSuitColored, UANSICHAR c)
    {
        return isSuitColored && !IsValueIdentifier(c) && !IsBorderIdentifier(c) && !IsExtraIdentifier(c);
    }
}

void ConsoleRenderer::SubmitFrame(GENSTRING frame)
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

void ConsoleRenderer::StageCardRow(GENSTRING& buff, int cardOffset, int row, bool isSuitColored) const
{
    UANSICSTR data = (UANSICSTR)s_cardsData;
    UANSICSTR mask = (UANSICSTR)s_cardsData + (Card::DECK_SIZE * CARD_AREA);
    UANSICHAR colorCode = (UANSICHAR)data[sizeof(s_cardsData) - 1];

    std::array<GENCHAR, 16> ansiColor{};
    std::array<GENCHAR, 16> ansiReset{};
    
    std::format_to(ansiColor.data(), GENTEXT("\x1B[{}m"), colorCode);
    std::format_to(ansiReset.data(), GENTEXT("\x1B[0m"));

    int rowOffset = row * CARD_WIDTH;
    
    for (int col = 0; col < CARD_WIDTH; col++)
    {
        UANSICHAR c = *(data + cardOffset + rowOffset + col);

        buff += (IsColorable(isSuitColored, c) && mask[rowOffset + col]) ? ansiColor.data() : ansiReset.data();
        buff += ConvertCharToEncoded(c);
    }
}

static bool IsColorCard(const Card& card)
{
    return card.suit.value == Card::SuitID::Hearts || card.suit.value == Card::SuitID::Diamonds;
}

void ConsoleRenderer::OnDisplayCard(const Card& card)
{
    int cardOffset = GetCardOffset(card);
    GENSTRING frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * 16);

    for (int row = 0; row < CARD_HEIGHT; row++)
    {
        StageCardRow(frame, cardOffset, row, IsColorCard(card));
        frame += GENTEXT('\n');
    }

    SubmitFrame(std::move(frame));
}

void ConsoleRenderer::OnDisplayHand(const Hand& hand)
{
    //Clear();
    GENSTRING frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * hand.GetCardCount() * 16);

    for (int row = 0; row < CARD_HEIGHT; row++)
    {
        for (int index = 0; index < hand.GetCardCount(); index++)
        {
            const Card* card = hand.GetCard(index).GetCard();
            StageCardRow(frame, GetCardOffset(*card), row, IsColorCard(*card));
        }

        frame += L'\n';
    }

    SubmitFrame(std::move(frame));
}

void ConsoleRenderer::StageDeckSingleRow(GENSTRING& frame, const Deck& deck)
{
    for (int index = 0; index < Card::DECK_SIZE; index++)
    {
        CardHandle handle = deck.GetCard(index);
        const Card* card = handle.GetCard();
        
        for (int row = 0; row < CARD_HEIGHT; row++)
        {
            StageCardRow(frame, GetCardOffset(*card), row, IsColorCard(*card));
            frame += L'\n';
        }
    }
}

void ConsoleRenderer::StageDeckChunked(GENSTRING& frame, const Deck& deck)
{
    for (int chunk = 0; chunk < Card::DECK_SIZE; chunk += Card::NUM_FACES)
    {
        for (int row = 0; row < CARD_HEIGHT; row++)
        {
            for (int index = 0; index < Card::NUM_FACES; index++)
            {
                CardHandle handle = deck.GetCard(chunk + index);
                const Card* card = handle.GetCard();

                StageCardRow(frame, GetCardOffset(*card), row, IsColorCard(*card));
            }

            frame += L'\n';
        }
    }
}

void ConsoleRenderer::OnDisplayDeck(const Deck& deck, bool bOnSingleRow)
{
    Clear();
    GENSTRING frame;
    frame.reserve(CARD_HEIGHT * CARD_WIDTH * Card::DECK_SIZE * 16);

    bOnSingleRow ? StageDeckSingleRow(frame, deck) : StageDeckChunked(frame, deck);

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

void ConsoleRenderer::Print(const GENSTRING& text)
{
    SubmitFrame(text);
}

void ConsoleRenderer::Println(const GENSTRING& text)
{
    Print(text + GENTEXT('\n'));
}