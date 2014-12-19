//
//  LocalisedText.h
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

#ifndef _CHILLISOURCE_CORE_LOCALISATION_LOCALISEDTEXT_H_
#define _CHILLISOURCE_CORE_LOCALISATION_LOCALISEDTEXT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>

#include <unordered_map>

namespace ChilliSource
{
	namespace Core
	{
		//----------------------------------------------------------------------
		/// Holds a resource loaded from the master text for a particular language.
		/// The text comprises of Ids which act as the key and the text value.
        ///
        /// All localised text is in UTF-8 encoding format
		///
		/// @author S Downie
		//----------------------------------------------------------------------
		class LocalisedText final : public Resource
		{
		public:
            
			CS_DECLARE_NAMEDTYPE(LocalisedText);

			//---------------------------------------------------------------------
			/// @author S Downie
			///
			/// @param Interface Id
			///
			/// @return Whether object is of given type
			//---------------------------------------------------------------------
			bool IsA(InterfaceIDType in_interfaceId) const override;
			//---------------------------------------------------------------------
			/// Populate the resource with the given keys and values
			///
			/// @author S Downie
			///
			/// @param A map containing the localised text. The text is in UTF-8
            /// format.
			//---------------------------------------------------------------------
			void Build(const std::unordered_map<std::string, std::string>& in_textMap);
            //---------------------------------------------------------------------
            /// @author Ian Copland
            ///
            /// @param The string key.
            ///
            /// @return Whether or not this has the given key.
            //---------------------------------------------------------------------
            bool Contains(const std::string& in_key) const;
            //---------------------------------------------------------------------
            /// @author S Downie
            ///
            /// @param String key
            ///
            /// @return String containing UTF8 codepoints that maps to this key
            //---------------------------------------------------------------------
            const std::string& GetText(const std::string& in_key) const;

		private:

			friend class ResourcePool;
			//---------------------------------------------------------------------
			/// Factory method to create empty text resource. Only called
			/// by the resource pool
			///
			/// @author S Downie
			///
			/// @return Ownership of resource
			//---------------------------------------------------------------------
			static LocalisedTextUPtr Create();
			//---------------------------------------------------------------------
			/// Private constructor to enforce use of factory create method
			///
			/// @author S Downie
			//---------------------------------------------------------------------
			LocalisedText() = default;
            
        private:
            
			std::unordered_map<std::string, std::string> m_text;
		};
	}
}

#endif