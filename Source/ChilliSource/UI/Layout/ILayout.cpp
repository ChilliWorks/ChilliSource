//
//  ILayout.cpp
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

#include <ChilliSource/UI/Layout/ILayout.h>

#include <ChilliSource/UI/Layout/GridLayout.h>
#include <ChilliSource/UI/Layout/HListLayout.h>
#include <ChilliSource/UI/Layout/LayoutType.h>
#include <ChilliSource/UI/Layout/VListLayout.h>

namespace ChilliSource
{
    namespace UI
    {
        //----------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------
        std::vector<PropertyMap::PropertyDesc> ILayout::GetPropertyDescs(LayoutType in_type)
        {
            switch(in_type)
            {
                case LayoutType::k_none:
                {
                    std::vector<PropertyMap::PropertyDesc> propertyDescs = {{PropertyType::k_string, "Type"}};
                    return propertyDescs;
                }
                case LayoutType::k_grid:
                    return GridLayout::GetPropertyDescs();
                case LayoutType::k_verticalList:
                    return VListLayout::GetPropertyDescs();
                case LayoutType::k_horizontalList:
                    return HListLayout::GetPropertyDescs();
            }
            
            return std::vector<PropertyMap::PropertyDesc>();
        }
    }
}