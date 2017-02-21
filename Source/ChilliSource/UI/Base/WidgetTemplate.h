//
//  WidgetTemplate.h
//  ChilliSource
//  Created by Scott Downie on 30/07/2014.
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETTEMPLATE_H_
#define _CHILLISOURCE_UI_BASE_WIDGETTEMPLATE_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/UI/Base/WidgetDesc.h>

namespace ChilliSource
{
    //-------------------------------------------------------------
    /// A resource that holds the description for creating new
    /// widget instances. A widget template is used to create
    /// instances of mutable UI i.e. screens. Widgets created
    /// from templates are fully defined usually with drawables,
    /// sizes, positions, etc and are ready for adding to the canvas.
    ///
    /// @author S Downie
    //-------------------------------------------------------------
    class WidgetTemplate final : public Resource
    {
    public:
        CS_DECLARE_NAMEDTYPE(WidgetTemplate);
        
        //-------------------------------------------------------
        /// Is the object of the given interface type.
        ///
        /// @author S Downie
        ///
        /// @param Interface type to query
        ///
        /// @return Whether the object is of given type
        //-------------------------------------------------------
        bool IsA(InterfaceIDType in_interfaceId) const override;
        //-------------------------------------------------------
        /// Build the resource which consists of the description
        /// of a widget and all its children
        ///
        /// @author S Downie
        ///
        /// @param The widget description.
        //-------------------------------------------------------
        void Build(const WidgetDesc& in_desc);
        //-------------------------------------------------------
        /// @author S Downie
        ///
        /// @return Hierarchy description
        //-------------------------------------------------------
        const WidgetDesc& GetWidgetDesc() const;
        
    private:
        
        friend class ResourcePool;
        //-------------------------------------------------------
        /// Factory method
        ///
        /// @author S Downie
        ///
        /// @return New backend with ownership transferred
        //-------------------------------------------------------
        static WidgetTemplateUPtr Create();
        //-------------------------------------------------------
        /// Private constructor to force use of factory method
        ///
        /// @author S Downie
        //-------------------------------------------------------
        WidgetTemplate();
        
    private:
        
        WidgetDesc m_desc;
    };
}

#endif

