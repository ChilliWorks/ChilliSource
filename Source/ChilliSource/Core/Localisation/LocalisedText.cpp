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
		void LocalisedText::Build(const std::unordered_map<std::string, std::string>& in_textMap)
		{
			m_text = in_textMap;
		}
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        bool LocalisedText::Contains(const std::string& in_key) const
        {
            return (m_text.find(in_key) != m_text.end());
        }
        //---------------------------------------------------------------------
        //---------------------------------------------------------------------
        const std::string& LocalisedText::GetText(const std::string& in_key) const
        {
			auto it = m_text.find(in_key);

			if (it != m_text.end())
			{
				return it->second;
			}

			CS_LOG_ERROR("No localised text for key: " + in_key);
			return StringUtils::BLANK;
        }
	}
}
