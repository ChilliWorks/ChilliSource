//
//  WidgetDesc.h
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETDESC_H_
#define _CHILLISOURCE_UI_BASE_WIDGETDESC_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/Colour.h>
#include <ChilliSource/Core/Math/UnifiedCoordinates.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Rendering/Base/AlignmentAnchors.h>
#include <ChilliSource/UI/Base/PropertyType.h>
#include <ChilliSource/UI/Base/SizePolicy.h>
#include <ChilliSource/UI/Drawable/DrawableType.h>
#include <ChilliSource/UI/Layout/LayoutType.h>

namespace ChilliSource
{
    namespace UI
    {
        //-------------------------------------------------------------
        /// A resource that holds the description for creating new
        /// widgets
        ///
        /// @author S Downie
        //-------------------------------------------------------------
        class WidgetDesc final : public Core::Resource
        {
        public:
            CS_DECLARE_NAMEDTYPE(WidgetDesc);
            
            //----------------------------------------------------
            /// Holds the definition of a custom property.
            ///
            /// @author S Downie
            //----------------------------------------------------
            struct CustomPropertyDesc
            {
                PropertyType m_type;
                std::string m_name;
            };
            //----------------------------------------------------
            /// Holds the definition of layout properties.
            ///
            /// @author S Downie
            //----------------------------------------------------
            struct LayoutPropertiesDesc
            {
                LayoutType m_layoutType = LayoutType::k_none;
                std::vector<CustomPropertyDesc> m_properties;
            };
            //----------------------------------------------------
            /// Holds the definition of drawable properties.
            ///
            /// @author S Downie
            //----------------------------------------------------
            struct DrawablePropertiesDesc
            {
                DrawableType m_drawableType = DrawableType::k_none;
                std::vector<CustomPropertyDesc> m_properties;
            };
            //-------------------------------------------------------
            /// Holds the values of the default properties for a widget
            ///
            /// @author S Downie
            //-------------------------------------------------------
            struct DefaultPropertiesDesc
            {
                LayoutPropertiesDesc m_layoutDesc;
                DrawablePropertiesDesc m_drawableDesc;
                std::string m_name;
                Core::UnifiedVector2 m_localPosition;
                Core::UnifiedVector2 m_localSize;
                Core::Vector2 m_preferredSize = Core::Vector2::k_one;
                Core::Vector2 m_localScale = Core::Vector2::k_one;
                Core::Colour m_localColour;
                f32 m_localRotation = 0.0f;
                Rendering::AlignmentAnchor m_originAnchor = Rendering::AlignmentAnchor::k_middleCentre;
                Rendering::AlignmentAnchor m_parentalAnchor = Rendering::AlignmentAnchor::k_middleCentre;
                SizePolicy m_sizePolicy = SizePolicy::k_none;
                bool m_isVisible = true;
                bool m_isSubviewClippingEnabled = false;
            };
            //----------------------------------------------------
            /// Holds the description of the entire widget
            /// including child hierarchy
            ///
            /// @author S Downie
            //----------------------------------------------------
            struct HierarchyDesc
            {
                DefaultPropertiesDesc m_defaultProperties;
                std::vector<CustomPropertyDesc> m_customProperties;
                std::vector<HierarchyDesc> m_children;
            };
            //-------------------------------------------------------
            /// Is the object of the given interface type.
            ///
            /// @author S Downie
            ///
            /// @param Interface type to query
            ///
            /// @return Whether the object is of given type
            //-------------------------------------------------------
            bool IsA(Core::InterfaceIDType in_interfaceId) const override;
            //-------------------------------------------------------
            /// Build the resource which consists of the description
            /// of a widget and all its children
            ///
            /// @author S Downie
            ///
            /// @param Hierarchy desc
            //-------------------------------------------------------
            void Build(const HierarchyDesc& in_desc);
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Hierarchy description
            //-------------------------------------------------------
            const HierarchyDesc& GetHierarchyDesc() const;
            
        private:
            
            friend class Core::ResourcePool;
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static WidgetDescUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            WidgetDesc() = default;
            
        private:
            
            HierarchyDesc m_desc;
        };
    }
}

#endif

