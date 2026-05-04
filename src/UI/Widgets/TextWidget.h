#pragma once

#include <Renderer/Widget.h>
#include <Platform/Platform.h>

class TextWidget : public Widget
{
    const GENSTRING m_text{};
    bool m_bNewLine = false;

public:
    TextWidget(const GENSTRING text, bool bNewLine = false)
        : m_text(text), m_bNewLine(bNewLine) {
    }

    void Render() const override
    {
        if (!m_text.empty())
        {
            m_bNewLine ? ConsoleRenderer::Get().Println(m_text) : ConsoleRenderer::Get().Print(m_text);
        }
    }
};

#define PRINT(fmt, ...) \
    do { \
        TextWidget(std::format(GENTEXT(fmt), ##__VA_ARGS__)).Render(); \
    } while(0)

#define PRINTLN(fmt, ...) \
    do { \
        TextWidget(std::format(GENTEXT(fmt), ##__VA_ARGS__), true).Render(); \
    } while(0)