#pragma once

#include <Renderer/Renderer.h>
#include <Renderer/DisplayComponent.h>
#include <Platform/Platform.h>

class TextDisplayComp : public DisplayComponent
{
    const GENSTRING* m_text = nullptr;
    bool m_bNewLine = false;

protected:
    void OnDisplay() override
    {
        if (m_text)
        {
            OnDisplayText(*m_text, m_bNewLine);
        }
    }

    virtual void OnDisplayText(const GENSTRING& text, bool bNewLine = false) = 0;

public:
    virtual ~TextDisplayComp() = default;

    void DisplayText(const GENSTRING& text, bool bNewLine = false)
    {
        m_text = &text;
        m_bNewLine = bNewLine;
        Display();
        m_text = nullptr;
    }
};