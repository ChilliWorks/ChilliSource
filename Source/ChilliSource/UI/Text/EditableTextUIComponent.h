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

#ifndef _CHILLISOURCE_UI_TEXT_EDITABLETEXTUICOMPONENT_H_
#define _CHILLISOURCE_UI_TEXT_EDITABLETEXTUICOMPONENT_H_

#include <ChilliSource/Core/Container/Property/PropertyMap.h>
#include <ChilliSource/Core/Event/EventConnection.h>
#include <ChilliSource/Input/Pointer/Pointer.h>
#include <ChilliSource/Input/TextEntry/TextEntry.h>
#include <ChilliSource/UI/Base/UIComponent.h>
#include <ChilliSource/UI/Text/TextUIComponent.h>

namespace ChilliSource
{
    /// A component which can be added to a widget to provide an editable text
    /// field. Editable text contains the following properties:
    ///
    /// "InitialText": The text present in the text field upon creation.
    /// Defaults to nothing.
    ///
    /// "MaxCharacters": The maximum number of characters allowed in this text field.
    /// Defaults to 50.
    ///
    /// "MultilineDisabled": Setting this to True will disable the display of newlines
    /// within the text field. This is False by default.
    /// 
    /// "InputType": Keyboard type; currently supported values are "Text" and "Numeric".
    /// Defaults to "Text".
    ///
    /// "CapitalisationFormat": Capitalisation format for the virtual keyboard.
    /// Currently supported values are: "None", "Words", "Sentences" and "All".
    /// Defaults to "Sentences".
    ///
    class EditableTextUIComponent final : public UIComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(EditableTextUIComponent);

        /// @return The list of properties supported by an editable text component.
        ///
        static const std::vector<PropertyMap::PropertyDesc>& GetPropertyDescs() noexcept;

        /// Allows querying of whether or not the component implements the
        /// interface associated with the given interface Id. Typically
        /// this won't be called directly, instead the templated version
        /// IsA<Interface>() should be used.
        ///
        /// @param 
        ///     The interface Id.
        ///
        /// @return Whether the object implements the given interface.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;

        /// @return The initial text present in the field upon creation.
        ///
        const std::string& GetInitialText() const noexcept;

        /// @return The maximum number of characters allowed in the field.
        ///
        s32 GetMaxCharacters() const noexcept;

        /// @return Whether or not multi-line text is enabled.
        ///
        bool GetMultilineDisabled() const noexcept;

        /// @return The keyboard input type.
        ///
        const ChilliSource::TextEntryType&  GetInputType() const noexcept;

        /// @return The capitalisation format.
        ///
        const ChilliSource::TextEntryCapitalisation& GetCapitalisationFormat() const noexcept;

        /// Set initial text present in the field.
        /// 
        /// @param text
        ///     The desired text.
        ///
        void SetInitialText(const std::string& text) noexcept;

        /// Set maximum number of characters allowed in the field.
        ///
        /// @param numCharacters
        ///     The maximum number of characters required.
        ///
        void SetMaxCharacters(s32 numCharacters) noexcept;

        /// Set whether multi-line text is disabled or not.
        ///
        /// @param disable
        ///     Whether or not to disable multiline text.
        ///
        void SetMultilineDisabled(bool disable) noexcept;

        /// Set keyboard input type.
        ///
        /// @param type
        ///     The desired keyboard input type.
        ///
        void SetInputType(ChilliSource::TextEntryType type) noexcept;

        /// Set capitalisation format. 
        ///
        /// @param format
        ///     The desired capitalisation format.
        ///
        void SetCapitalisationFormat(ChilliSource::TextEntryCapitalisation format) noexcept;

        /// Enables text entry and invokes virtual keyboard on mobile devices.
        ///
        void Activate() noexcept;

        /// Disables text entry and dismisses virtual keyboard on mobile devices.
        ///
        void Deactivate() noexcept;

    private:
        friend class UIComponentFactory;

        /// Constructor that builds the component from key-value properties.
        /// The properties used to create a highlight component are described
        /// in the class documentation.
        /// @param componentName
        ///     The component name.
        /// @param properties
        ///     The property map.
        ///
        EditableTextUIComponent(const std::string& componentName, const PropertyMap& properties) noexcept;

        /// Called when text entry is active and the text has been changed.
        ///
        /// @return If the new text is valid or not.
        ///
        bool OnTextChanged(const std::string& newText) noexcept;
        
        /// Called when the virtual keyboard is dismissed.
        ///
        void OnTextEntryDismissed() noexcept;

        /// Called when the component is first added to the owning widget.
        ///
        void OnInit() noexcept override;

        /// Called when a pointer is released inside the bounds of the owning
        /// widget. This will toggle the text entry & virtual keyboard.
        ///
        /// @param widget
        ///     The owning widget.
        /// @param pointer
        ///     The pointer that was pressed.
        /// @param inputType
        ///     The type of input.
        ///
        void OnReleasedInside(Widget* widget, const Pointer& pointer, Pointer::InputType inputType) noexcept;

        /// Called when a pointer is released outside the bounds of the owning
        /// widget. This will disable the text entry & virtual keyboard.
        ///
        /// @param widget
        ///     The owning widget.
        /// @param pointer
        ///     The pointer that was pressed.
        /// @param inputType
        ///     The type of input.
        ///
        void OnReleasedOutside(Widget* widget, const Pointer& pointer, Pointer::InputType inputType) noexcept;

        /// Called when the owning widget is destructed.
        ///
        void OnDestroy() noexcept override;

        TextUIComponent* m_textComponent = nullptr;
        TextEntry* m_textEntrySystem = nullptr;

        bool m_active = false;
        bool m_multilineDisabled = false;
        s32 m_maxCharacters = 50;
        std::string m_initialText = "";
        ChilliSource::TextEntryType m_inputType = ChilliSource::TextEntryType::k_text;
        ChilliSource::TextEntryCapitalisation m_capitalisationFormat = ChilliSource::TextEntryCapitalisation::k_sentences;

        EventConnectionUPtr m_releasedInsideConnection;
        EventConnectionUPtr m_releasedOutsideConnection;
    };
}

#endif