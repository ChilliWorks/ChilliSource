/*
 *  LocalisedText.h
 *  moFlo
 *
 *  Created by Scott Downie on 07/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#ifndef _MO_FLO_CORE_LOCALISED_TEXT_H_
#define _MO_FLO_CORE_LOCALISED_TEXT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Container/HashedArray.h>
#include <ChilliSource/Core/String/UTF8String.h>

namespace ChilliSource
{
	namespace Core
	{
		typedef s32 LocalisedTextKey;
		
		class LocalisedText 
		{
		public:
            
            //-----------------------------------------------------
			/// Refreshes the master text. This should be called
            /// whenever the master text changes on disc, for example
            /// after a DLC update.
            ///
            /// @author I Copland
            ///
            /// @param The storage location of the master text.
            /// @param Directory path excluding name, i.e. if root then
            /// just "" should be passed.
            //-----------------------------------------------------
            static void RefreshMasterText(StorageLocation ineStorageLocation, const std::string& instrDirectory);
			//---------------------------------------------------------------------
			/// Get Text
			///
			/// @param The key corresponding to some localised text
			/// @return The localised text string
			//---------------------------------------------------------------------
			static const UTF8String& GetText(const LocalisedTextKey &inKey);
            //---------------------------------------------------------------------
			/// Get Text
			///
			/// @param String key
			/// @return The localised text string
			//---------------------------------------------------------------------
			static const UTF8String& GetText(const std::string &instrID);
			//----------------------------------------------------------------------------
			/// Load Text From File
			///
			/// Load the font from the external file
			/// @param Storage location to load from
			/// @param File path
			/// @param File name
			/// @return Success
			//----------------------------------------------------------------------------
			static bool LoadTextFromFile(StorageLocation ineLocation, const std::string & inFilePath, const std::string & inFileName);
			
		private:
   
            //----------------------------------------------------------------------------
            /// Load Localised Text
            ///
            /// Load the localised text from the external file
            /// @param File object for the locale file
            /// @return Success
            //----------------------------------------------------------------------------
            static bool LoadLocalisedText(FileStreamSPtr& incLocaleFile);
            //----------------------------------------------------------------------------
            /// Load Text ID
            ///
            /// Load the text ids from the external file
            /// @param File object for the text ids file
            /// @return Success
            //----------------------------------------------------------------------------
            static bool LoadTextID(FileStreamSPtr& incIDFile);
            
        private:
            
            typedef HashedArray<std::string, LocalisedTextKey> IDToLookupIndex;
            static IDToLookupIndex* mpTextLookup;
			static UTF8String * mpText;
            static u32 mudwLineCount;
		};
	}
}

#endif