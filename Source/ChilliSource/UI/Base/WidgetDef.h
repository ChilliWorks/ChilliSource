//
//  WidgetDef.h
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

#ifndef _CHILLISOURCE_UI_BASE_WIDGETDEF_H_
#define _CHILLISOURCE_UI_BASE_WIDGETDEF_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/UI/Base/WidgetHierarchyDesc.h>

namespace ChilliSource
{
    namespace UI
    {
        //-------------------------------------------------------------
        /// A resource that holds the description for defining new
        /// widget types. Widgets created from defines have provate implementation
        /// details such as child widgets and are supposed to be treated as
        /// as single widget rather than as a screen. Widgets created from
        /// defines rarely have any properties set only their behaviour and
        /// hiearchy is fixed and there they require positioning and drawables
        /// etc before they can be added to the canvas.
        ///
        /// @author S Downie
        //-------------------------------------------------------------
        class WidgetDef final : public Core::Resource
        {
        public:
            CS_DECLARE_NAMEDTYPE(WidgetDef);
            
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
            /// @param Lua source for widget behaviour
            //-------------------------------------------------------
            void Build(const WidgetHierarchyDesc& in_desc, const Scripting::LuaSourceCSPtr& in_behaviourSource);
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Hierarchy description
            //-------------------------------------------------------
            const WidgetHierarchyDesc& GetHierarchyDesc() const;
            //-------------------------------------------------------
            /// @author S Downie
            ///
            /// @return Lua source for widget behaviour
            //-------------------------------------------------------
            const Scripting::LuaSourceCSPtr& GetBehaviourSource() const;
            
        private:
            
            friend class Core::ResourcePool;
            //-------------------------------------------------------
            /// Factory method
            ///
            /// @author S Downie
            ///
            /// @return New backend with ownership transferred
            //-------------------------------------------------------
            static WidgetDefUPtr Create();
            //-------------------------------------------------------
            /// Private constructor to force use of factory method
            ///
            /// @author S Downie
            //-------------------------------------------------------
            WidgetDef() = default;
            
        private:
            
            WidgetHierarchyDesc m_desc;
            
            Scripting::LuaSourceCSPtr m_behaviourSource;
        };
    }
}

#endif

