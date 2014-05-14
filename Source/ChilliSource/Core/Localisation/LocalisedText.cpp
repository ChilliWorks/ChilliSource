//
//  LocalisedText.cpp
//  Chilli Source
//  Created by Scott Downie on 07/12/2010.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2010 Tag Games Limited
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

#include <ChilliSource/Core/Localisation/LocalisedText.h>

#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Core
	{	
		CS_DEFINE_NAMEDTYPE(LocalisedText);

		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		LocalisedTextUPtr LocalisedText::Create()
		{
			return LocalisedTextUPtr(new LocalisedText());
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		bool LocalisedText::IsA(InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == LocalisedText::InterfaceID;
		}
		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		void LocalisedText::Build(const std::vector<std::string>& in_keys, const std::vector<UTF8String>& in_values)
		{
			CS_ASSERT(in_keys.size() == in_values.size(), "Localised text must have equal numbers of keys and values");

			u32 numTexts = in_keys.size();
			for (u32 i = 0; i < numTexts; ++i)
			{
				m_text.insert(std::make_pair(in_keys[i], in_values[i]));
			}
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const UTF8String& LocalisedText::GetText(const std::string& in_key) const
        {
			auto it = m_text.find(in_key);

			if (it != m_text.end())
			{
				return it->second;
			}

			CS_LOG_ERROR("No localised text for key: " + in_key);
			return StringUtils::UTF8_BLANK;
        }
	}
}
