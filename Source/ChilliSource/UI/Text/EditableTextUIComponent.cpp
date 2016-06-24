//  The MIT License (MIT)
//
//  Copyright (c) 2016 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include <ChilliSource/UI/Text/EditableTextUIComponent.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Container/VectorUtils.h>
#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Container/Property/PropertyTypes.h>
#include <ChilliSource/Core/Delegate/MakeDelegate.h>
#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/PropertyTypes.h>

namespace ChilliSource
{
    namespace
    {
        const char k_initialTextKey[] = "InitialText";
        const char k_maxCharactersKey[] = "MaxCharacters";
        const char k_multilineDisabledKey[] = "MultilineDisabled";
        const char k_inputTypeKey[] = "InputType";
        const char k_capitalisationFormatKey[] = "CapitalisationFormat";

        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            { PropertyTypes::String(), k_initialTextKey },
            { PropertyTypes::Int(), k_maxCharactersKey },
            { PropertyTypes::Bool(), k_multilineDisabledKey },
            { PropertyTypes::InputType(), k_inputTypeKey },
            { PropertyTypes::CapitalisationFormat(), k_capitalisationFormatKey }
        };
    }

    CS_DEFINE_NAMEDTYPE(EditableTextUIComponent);

    //-------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& EditableTextUIComponent::GetPropertyDescs() noexcept
    {
        return k_propertyDescs;
    }

    //-------------------------------------------------------
    EditableTextUIComponent::EditableTextUIComponent(const std::string& componentName, const PropertyMap& properties) noexcept
        : UIComponent(componentName) 
    {
        RegisterProperty<std::string>(PropertyTypes::String(), k_initialTextKey, MakeDelegate(this, &EditableTextUIComponent::GetInitialText), MakeDelegate(this, &EditableTextUIComponent::SetInitialText));
        RegisterProperty<s32>(PropertyTypes::Int(), k_maxCharactersKey, MakeDelegate(this, &EditableTextUIComponent::GetMaxCharacters), MakeDelegate(this, &EditableTextUIComponent::SetMaxCharacters));
        RegisterProperty<bool>(PropertyTypes::Bool(), k_multilineDisabledKey, MakeDelegate(this, &EditableTextUIComponent::GetMultilineDisabled), MakeDelegate(this, &EditableTextUIComponent::SetMultilineDisabled));
        RegisterProperty<ChilliSource::TextEntryType>(PropertyTypes::InputType(), k_inputTypeKey, MakeDelegate(this, &EditableTextUIComponent::GetInputType), MakeDelegate(this, &EditableTextUIComponent::SetInputType));
        RegisterProperty<ChilliSource::TextEntryCapitalisation>(PropertyTypes::CapitalisationFormat(), k_capitalisationFormatKey, MakeDelegate(this, &EditableTextUIComponent::GetCapitalisationFormat), MakeDelegate(this, &EditableTextUIComponent::SetCapitalisationFormat));
        ApplyRegisteredProperties(properties);
    }

    //--------------------------------------------------------
    bool EditableTextUIComponent::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return (UIComponent::InterfaceID == interfaceId || EditableTextUIComponent::InterfaceID == interfaceId);
    }

    //--------------------------------------------------------
    const std::string& EditableTextUIComponent::GetInitialText() const noexcept
    {
        return m_initialText;
    }

    //--------------------------------------------------------
    s32 EditableTextUIComponent::GetMaxCharacters() const noexcept
    {
        return m_maxCharacters;
    }

    //--------------------------------------------------------
    bool EditableTextUIComponent::GetMultilineDisabled() const noexcept
    {
        return m_multilineDisabled;
    }

    //--------------------------------------------------------
    const ChilliSource::TextEntryType& EditableTextUIComponent::GetInputType() const noexcept
    {
        return m_inputType;
    }

    //--------------------------------------------------------
    const ChilliSource::TextEntryCapitalisation& EditableTextUIComponent::GetCapitalisationFormat() const noexcept
    {
        return m_capitalisationFormat;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::SetInitialText(const std::string& text) noexcept
    {
        m_initialText = text;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::SetMaxCharacters(s32 numCharacters) noexcept
    {
        m_maxCharacters = numCharacters;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::SetMultilineDisabled(bool disabled) noexcept
    {
        m_multilineDisabled = disabled;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::SetInputType(ChilliSource::TextEntryType type) noexcept
    {
        m_inputType = type;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::SetCapitalisationFormat(ChilliSource::TextEntryCapitalisation format) noexcept
    {
        m_capitalisationFormat = format;
    }

    //--------------------------------------------------------
    void EditableTextUIComponent::Activate() noexcept
    {
        CS_ASSERT(!m_active, "Can't activate text entry if it is already active.");
        m_active = true;

        if (m_textEntrySystem->IsActive())
        {
            m_textEntrySystem->Deactivate();
        }

        m_textEntrySystem->Activate(m_initialText, m_inputType, m_capitalisationFormat, MakeDelegate(this, &EditableTextUIComponent::OnTextChanged), MakeDelegate(this, &EditableTextUIComponent::OnTextEntryDismissed));
    }

    //---------------------------------------------------------
    void EditableTextUIComponent::Deactivate() noexcept
    {
        CS_ASSERT(m_active, "Can't deactivate text entry if it is not active.");
        m_active = false;

        m_textEntrySystem->Deactivate();
    }

    //----------------------------------------------------------
    bool EditableTextUIComponent::OnTextChanged(const std::string& newText) noexcept
    {
        // Make sure text is within set size, otherwise reject.
        if (newText.size() > m_maxCharacters)
        {
            return false;
        }

        // Make sure the font can handle all the characters going in, otherwise reject text.
        for (auto character : newText)
        {
            ChilliSource::Font::CharacterInfo info;
            if (!m_textComponent->GetFont()->TryGetCharacterInfo(character, info))
            {
                return false;
            }
        }

        m_textComponent->SetText(newText);

        return true;
    }

    //----------------------------------------------------------
    void EditableTextUIComponent::OnTextEntryDismissed() noexcept
    {
        // Update activation state.
        m_active = false;
    }

    //-----------------------------------------------------------
    void EditableTextUIComponent::OnReleasedInside(Widget* widget, const Pointer& pointer, Pointer::InputType inputType) noexcept
    {
        // Toggle activation status when element is pressed.
        if (m_active)
        {
            Deactivate();
        }
        else
        {
            Activate();
        }
    }

    //-----------------------------------------------------------
    void EditableTextUIComponent::OnReleasedOutside(Widget* widget, const Pointer& pointer, Pointer::InputType inputType) noexcept
    {
        // If user presses outside of element, defocus and deactivate.
        if (m_active)
        {
            Deactivate();
        }
    }

    //------------------------------------------------------------
    void EditableTextUIComponent::OnInit() noexcept
    {
        m_textEntrySystem = ChilliSource::Application::Get()->GetSystem<ChilliSource::TextEntry>();
        CS_ASSERT(m_textEntrySystem, "No active text entry system found.");
        m_textEntrySystem->SetTextBuffer(m_initialText);

        m_textComponent = GetWidget()->GetComponent<TextUIComponent>();
        CS_ASSERT(m_textComponent, "No text component found in editable text widget.");
        m_textComponent->SetText(m_initialText);

        m_releasedInsideConnection = GetWidget()->GetReleasedInsideEvent().OpenConnection(MakeDelegate(this, &EditableTextUIComponent::OnReleasedInside));
        m_releasedOutsideConnection = GetWidget()->GetReleasedOutsideEvent().OpenConnection(MakeDelegate(this, &EditableTextUIComponent::OnReleasedOutside));
    }

    //-------------------------------------------------------------
    void EditableTextUIComponent::OnDestroy() noexcept
    {
        if (m_active)
        {
            Deactivate();
        }

        // Clear text from buffer.
        m_textEntrySystem->SetTextBuffer("");

        m_releasedInsideConnection.reset();
        m_releasedOutsideConnection.reset();
    }
}