//
//  UILayout.h
//  Chilli Source
//  Created by Scott Downie on 18/04/2014.
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

#ifndef _CHILLISOURCE_UI_LAYOUT_UILAYOUT_H_
#define _CHILLISOURCE_UI_LAYOUT_UILAYOUT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Base/QueryableInterface.h>

#include <vector>

namespace ChilliSource
{
    //------------------------------------------------------------------------------
    /// Interface for laying out widgets. This is used by UILayout UIComponent to
    /// apply the described layout.
    ///
    /// @author S Downie
    //------------------------------------------------------------------------------
    class UILayout : public QueryableInterface
    {
    public:
        CS_DECLARE_NAMEDTYPE(ILayout);
        CS_DECLARE_NOCOPY(UILayout);
        //------------------------------------------------------------------------------
        /// Constructor
        ///
        /// @author S Downie
        ///
        /// @param The layout component that owns this layout.
        //------------------------------------------------------------------------------
        UILayout(LayoutUIComponent* in_layoutComponent);
        //------------------------------------------------------------------------------
        /// Virtual destructor
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual ~UILayout() {}
        
    protected:
        //------------------------------------------------------------------------------
        /// @author Ian Copland
        ///
        /// @return The owning component.
        //------------------------------------------------------------------------------
        LayoutUIComponent* GetComponent();
        
    private:
        friend class LayoutUIComponent;
        //------------------------------------------------------------------------------
        /// Create the layout sizes and positions based on the current layout properties
        /// and the owning widget.
        ///
        /// @author S Downie
        //------------------------------------------------------------------------------
        virtual void BuildLayout() = 0;
        //------------------------------------------------------------------------------
        /// Get the final size of the widget that occupies the space at the given index
        ///
        /// @author S Downie
        ///
        /// @param Space index
        ///
        /// @return Final absolute screen size
        //------------------------------------------------------------------------------
        virtual Vector2 GetSizeForIndex(u32 in_index) const = 0;
        //------------------------------------------------------------------------------
        /// Get the local position of the widget that occupies the space at the given
        /// index
        ///
        /// @author S Downie
        ///
        /// @param Space index
        ///
        /// @return Local position (aligned middle centre of the cell)
        //------------------------------------------------------------------------------
        virtual Vector2 GetPositionForIndex(u32 in_index) const = 0;
        
        LayoutUIComponent* m_component = nullptr;
    };
}

#endif