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

        const std::vector<PropertyMap::PropertyDesc> k_propertyDescs =
        {
            { PropertyTypes::String(), k_initialTextKey },
            { PropertyTypes::Int(), k_maxCharactersKey },
            { PropertyTypes::Bool(), k_multilineDisabledKey }
        };
    }

    CS_DEFINE_NAMEDTYPE(EditableTextUIComponent);

    //-------------------------------------------------------
    const std::vector<PropertyMap::PropertyDesc>& EditableTextUIComponent::GetPropertyDescs() noexcept
    {
        return k_propertyDescs;
    }

    //-------------------------------------------------------
    EditableTextUIComponent::EditableTextUIComponent(const std::string& componentName, const PropertyMap& properties)
        : UIComponent(componentName)
    {
        RegisterProperty<std::string>(PropertyTypes::String(), k_initialTextKey, MakeDelegate(this, &EditableTextUIComponent::GetInitialText), MakeDelegate(this, &EditableTextUIComponent::SetInitialText));
        RegisterProperty<s32>(PropertyTypes::Int(), k_maxCharactersKey, MakeDelegate(this, &EditableTextUIComponent::GetMaxCharacters), MakeDelegate(this, &EditableTextUIComponent::SetMaxCharacters));
        RegisterProperty<bool>(PropertyTypes::Bool(), k_multilineDisabledKey, MakeDelegate(this, &EditableTextUIComponent::GetMultilineDisabled), MakeDelegate(this, &EditableTextUIComponent::SetMultilineDisabled));
        ApplyRegisteredProperties(properties);
    }
}