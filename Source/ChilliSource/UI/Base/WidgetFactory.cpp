//
//  WidgetFactory.cpp
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


#include <ChilliSource/UI/Base/WidgetFactory.h>

#include <ChilliSource/UI/Base/Widget.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>
#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/HListLayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>
#include <ChilliSource/UI/Layout/VListLayout.h>

namespace ChilliSource
{
	namespace UI
	{
		CS_DEFINE_NAMEDTYPE(WidgetFactory);
        
        namespace
        {
            //---------------------------------------------------------------------------
            /// Create the layout class based on the given description
            ///
            /// @author S Downie
            ///
            /// @param Layout desc
            ///
            /// @return Layout or nullptr
            //---------------------------------------------------------------------------
            ILayoutUPtr CreateLayout(const WidgetDesc::LayoutPropertiesDesc& in_desc)
            {
                //TODO: Handle the properties
                switch(in_desc.m_layoutType)
                {
                    case LayoutType::k_none:
                        return nullptr;
                    case LayoutType::k_grid:
                        return ILayoutUPtr(new GridLayout());
                    case LayoutType::k_horizontalList:
                        return ILayoutUPtr(new HListLayout());
                    case LayoutType::k_verticalList:
                        return ILayoutUPtr(new VListLayout());
                }
                
                return nullptr;
            }
            //---------------------------------------------------------------------------
            /// Recursively create the widget hierarchy from the hierarchy desc
            ///
            /// @author S Downie
            ///
            /// @param Hierarchy desc
            ///
            /// @return Widget
            //---------------------------------------------------------------------------
            WidgetSPtr CreateRecursive(const WidgetDesc::HierarchyDesc& in_hierarchyDesc)
            {
                CSUI::WidgetSPtr widget(std::make_shared<CSUI::Widget>(in_hierarchyDesc.m_defaultProperties, in_hierarchyDesc.m_customProperties));
                
                widget->SetLayout(CreateLayout(in_hierarchyDesc.m_defaultProperties.m_layoutDesc));
                
                for(const auto& childHierarchyDesc : in_hierarchyDesc.m_children)
                {
                    widget->AddInternalWidget(CreateRecursive(childHierarchyDesc));
                }
                
                return widget;
            }
        }
        
        //--------------------------------------------------------
        //--------------------------------------------------------
        WidgetFactoryUPtr WidgetFactory::Create()
        {
            return WidgetFactoryUPtr(new WidgetFactory());
        }
        //--------------------------------------------------------
        //--------------------------------------------------------
		bool WidgetFactory::IsA(Core::InterfaceIDType in_interfaceId) const
        {
			return in_interfaceId == WidgetFactory::InterfaceID;
		}
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        WidgetSPtr WidgetFactory::Create(const WidgetDescCSPtr& in_desc)
        {
            return CreateRecursive(in_desc->GetHierarchyDesc());
        }
	}
}
