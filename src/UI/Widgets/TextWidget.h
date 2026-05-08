#pragma once

#include <Renderer/Widget.h>
#include <Renderer/Renderer.h>
#include <Core/Macros/ErrorMacros.h>
#include "UI/TextDisplay.h"

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
        auto* comp = Renderer::Get().As<TextDisplayComp>();

        if (comp)
        {
            comp->DisplayText(m_text, m_bNewLine);
        }
    }
};

#define PRINT(fmt, ...)                                                \
    do {                                                               \
        TextWidget(std::format(GENTEXT(fmt), ##__VA_ARGS__)).Render(); \
    } while(0)

#define PRINTLN(fmt, ...)                                                    \
    do {                                                                     \
        TextWidget(std::format(GENTEXT(fmt), ##__VA_ARGS__), true).Render(); \
    } while(0)