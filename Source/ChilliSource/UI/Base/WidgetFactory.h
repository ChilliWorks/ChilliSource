//
//  WidgetFactory.h
//  Chilli Source
//  Created by Scott Downie on 25/07/2014.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2014 Tag Games Limited
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
//

#ifndef _CHILLISOURCE_UI_BASE_WIDGETFACTORY_H_
#define _CHILLISOURCE_UI_BASE_WIDGETFACTORY_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/System/AppSystem.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace UI
	{
        //--------------------------------------------------------
        /// System that has convenience methods for creating
        /// widgets
        ///
        /// @author S Downie
        //--------------------------------------------------------
		class WidgetFactory final : public Core::AppSystem
		{
		public:
            
			CS_DECLARE_NAMEDTYPE(WidgetFactory);

            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param Interface Id
            ///
            /// @return If the object is of the given interface type
            //---------------------------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //---------------------------------------------------------------------------
            /// Register the given widget definition so it is accessible via its type name.
            /// This allows UI files to reference the definition by a readable name
            ///
            /// @author S Downie
            ///
            /// @param Widget def resource
            //---------------------------------------------------------------------------
            void RegisterDefinition(const WidgetDefCSPtr& in_def);
            //---------------------------------------------------------------------------
            /// Get the widget definition that is keyed on the given name
            ///
            /// @author S Downie
            ///
            /// @param Name key
            ///
            /// @return Widget def resource
            //---------------------------------------------------------------------------
            WidgetDefCSPtr GetDefinition(const std::string& in_nameKey) const;
            //---------------------------------------------------------------------------
            /// Creates a new widget based on the given definition. The type of the
            /// widget is described by its children, behaviour and properties
            ///
            /// @author S Downie
            ///
            /// @param Widget def resource
            ///
            /// @return Widget with look and behaviour described by definition
            //---------------------------------------------------------------------------
            WidgetUPtr Create(const WidgetDefCSPtr& in_def) const;
            //---------------------------------------------------------------------------
            /// Creates a new widget based on the given template. The type of the
            /// widget is set within the template
            ///
            /// @author S Downie
            ///
            /// @param Widget template resource
            ///
            /// @return Widget
            //---------------------------------------------------------------------------
            WidgetUPtr Create(const WidgetTemplateCSPtr& in_template) const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new standard widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateWidget() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new image widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateImage() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new highlight button widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateHighlightButton() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new toggle button widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateToggleButton() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new label widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateLabel() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new layout widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateLayout() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new horizontal slider widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateHorizontalSlider() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new vertical slider widget.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateVerticalSlider() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new horizontal fill progress bar.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateHorizontalFillProgressBar() const;
            //---------------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @return A new vertical fill progress bar.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateVerticalFillProgressBar() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new horizontal stretch progress bar.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateHorizontalStretchProgressBar() const;
            //---------------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @return A new vertical stretch progress bar.
            //---------------------------------------------------------------------------
            WidgetUPtr CreateVerticalStretchProgressBar() const;
            
        private:
            
            friend class Core::Application;
            //---------------------------------------------------------------------------
            /// Creation method used by application to instantiate
            /// the system
            ///
            /// @author S Downie
            ///
            /// @return Ownership of new factory
            //---------------------------------------------------------------------------
			static WidgetFactoryUPtr Create();
            //---------------------------------------------------------------------------
            /// Private constructor to enforce the use of factory
            /// method
            ///
            /// @author S Downie
            //---------------------------------------------------------------------------
			WidgetFactory() = default;
            //---------------------------------------------------------------------------
            /// Called when the system is created in order to create the built-in
            /// widget types
            ///
            /// @author S Downie
            //---------------------------------------------------------------------------
            void OnInit() override;
            //---------------------------------------------------------------------------
            /// Recursively create the widget hierarchy from the hierarchy desc
            ///
            /// @author S Downie
            ///
            /// @param The widget definition.
            /// @param The widget description.
            ///
            /// @return The output widget
            //---------------------------------------------------------------------------
            WidgetUPtr CreateRecursive(const WidgetDefCSPtr& in_widgetDef, const WidgetDesc& in_widgetDesc) const;
            //---------------------------------------------------------------------------
            /// Called when the system is destroyed and will release
            /// any memory held
            ///
            /// @author S Downie
            //---------------------------------------------------------------------------
            void OnDestroy() override;
            
        private:
            
            ComponentFactory* m_componentFactory;
            std::unordered_map<std::string, WidgetDefCSPtr> m_widgetDefNameMap;
		};
	}
}

#endif
