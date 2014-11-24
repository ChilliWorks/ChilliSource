//
//  Component.cpp
//  Chilli Source
//  Created by Ian Copland on 14/11/2014.
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

#include <ChilliSource/UI/Base/Component.h>

namespace ChilliSource
{
    namespace UI
    {
        CS_DEFINE_NAMEDTYPE(Component);
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Component::Component(const std::string& in_name)
            : m_widget(nullptr), m_name(in_name)
        {
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const std::string& Component::GetName() const
        {
            return m_name;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        Widget* Component::GetWidget()
        {
            return m_widget;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        const Widget* Component::GetWidget() const
        {
            return m_widget;
        }
        //----------------------------------------------------------------
        //----------------------------------------------------------------
        void Component::SetWidget(Widget* in_widget)
        {
            CS_ASSERT(m_widget == nullptr, "Cannot change the owning widget on a component.");
            
            m_widget = in_widget;
        }
    }
}