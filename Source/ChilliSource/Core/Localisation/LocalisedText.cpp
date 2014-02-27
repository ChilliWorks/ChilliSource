/*
 *  LocalisedText.cpp
 *  moFlow
 *
 *  Created by Scott Downie on 07/12/2010.
 *  Copyright 2010 Tag Games. All rights reserved.
 *
 */

#include <ChilliSource/Core/Localisation/LocalisedText.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/String/StringUtils.h>

namespace ChilliSource
{
	namespace Core
	{	
        u32 CLocalisedText::mudwLineCount = 0;
        UTF8String* CLocalisedText::mpText = nullptr;
        CLocalisedText::IDToLookupIndex* CLocalisedText::mpTextLookup = nullptr;
        
		//---------------------------------------------------------------------
		/// Get Text
		///
		/// @param The key corresponding to some localised text
		/// @return The localised text string
		//---------------------------------------------------------------------
		const UTF8String& CLocalisedText::GetText(const LocalisedTextKey &inKey) 
		{
			if(inKey >= 0 && inKey < (s32)mudwLineCount)
			{
                const UTF8String& strText(mpText[inKey]);
				return strText;
			}

			return CStringUtils::UTF8_MISSING;
		}
        //---------------------------------------------------------------------
        /// Get Text
        ///
        /// @param String key
        /// @return The localised text string
        //---------------------------------------------------------------------
        const UTF8String& CLocalisedText::GetText(const std::string &instrID) 
        {
			CS_ASSERT(mpTextLookup, "Text Lookup IDs are missing");

            IDToLookupIndex::iterator it = mpTextLookup->Find(instrID);
            
            if(it != mpTextLookup->end())
            {
                return GetText(it->second);
            }
            
            CS_ERROR_LOG("Missing localised text with ID:" + instrID);
            return GetText(-1);
        }
		//----------------------------------------------------------------------------
		/// Load Text From File
		///
		/// Load the font from the external file
		/// @param Storage location to load from
		/// @param File path
		/// @param File name
		/// @return Success
		//----------------------------------------------------------------------------
        bool CLocalisedText::LoadTextFromFile(StorageLocation ineLocation, const std::string & inFilePath, const std::string & inFileName)
		{
            CS_SAFE_DELETE_ARRAY(mpText);
            CS_SAFE_DELETE(mpTextLookup);
            mudwLineCount = 0;
            
			FileStreamSPtr localFile = Application::GetFileSystemPtr()->CreateFileStream(ineLocation, inFilePath + inFileName, FileMode::k_read);
            // Load localised text
			if(LoadLocalisedText(localFile) == false)
            {
                return false;
            }
            
			FileStreamSPtr idFile = Application::GetFileSystemPtr()->CreateFileStream(ineLocation, inFilePath + "TagText.id", FileMode::k_read);
            // Load in string IDs
            if(LoadTextID(idFile) == false)
            {
                return false;
            }
            
            return true;
		}
        
        //----------------------------------------------------------------------------
        /// Load Localised Text
        ///
        /// Load the localised text from the external file
        /// @param File object for the locale file
        /// @return Success
        //----------------------------------------------------------------------------
        bool CLocalisedText::LoadLocalisedText(FileStreamSPtr& incLocaleFile)
        {
            //if bad, then bail!
			if (incLocaleFile == nullptr || incLocaleFile->IsBad() == true)
			{
				if(incLocaleFile->IsOpen())
                {
					incLocaleFile->Close();
                }
                
                return false;
			}
            
			// Check we have not seeked beyond file
			if(incLocaleFile->IsBad() == true)
			{
				if(incLocaleFile->IsOpen())
                {
					incLocaleFile->Close();
                }
                
                return false;
			}
            
            // Read file
			std::string strFile;
            incLocaleFile->GetAll(strFile);
            incLocaleFile->Close();
            
            // Loop through the entire file
            std::string::const_iterator it;
            
            // Get line count
            u32 udwLineCount = 0;
            u32 udwCurrentLine = 0;
            for(it = strFile.begin(); it != strFile.end(); ++it)
            {
                udwCurrentLine++;
                u8 byChar = *it;
                bool bNewLine = false;
                
                //insure that windows evil "\n\r" doesnt cause any problems
                if (byChar == '\n')
                {
                    std::string::const_iterator itNext = it + 1;
                    if (itNext != strFile.end())
                    {
                        if (*itNext == '\r')
                        {
                            it++;
                        }
                    }
                    bNewLine = true;
                }
                else if (byChar == '\r')
                {
                    std::string::const_iterator itNext = it + 1;
                    if (itNext != strFile.end())
                    {
                        if (*itNext == '\n')
                        {
                            it++;
                        }
                    }
                    bNewLine = true;
                }
                
                if(bNewLine == true)
                {
                    udwLineCount++;
                    udwCurrentLine = 0;
                }
            }
            
            // Catch if last line has no newline
            if(udwCurrentLine != 0)
            {
                udwLineCount++;
            }
            
            mudwLineCount = udwLineCount;
            mpText = new UTF8String[mudwLineCount];
            
            std::string strTextLine;
            udwCurrentLine = 0;
            for(it = strFile.begin(); it != strFile.end(); ++it)
            {
                u8 byChar = *it;
                bool bNewLine = false;
                
                //insure that windows evil "\n\r" doesnt cause any problems
			   if (byChar == '\n')
			   {
				   std::string::const_iterator itNext = it + 1;
				   if (itNext != strFile.end())
				   {
					   if (*itNext == '\r')
                       {
						   it++;
                       }
				   }
				   bNewLine = true;
			   }
			   else if (byChar == '\r')
			   {
				   std::string::const_iterator itNext = it + 1;
				   if (itNext != strFile.end())
				   {
					   if (*itNext == '\n')
                       {
						   it++;
                       }
				   }
				   bNewLine = true;
			   }

                if (!bNewLine)
                {
                    //We concatenate any '\''n' into '\n' at this point
                    if(byChar == '\\')
                    {
                        //Peek ahead for an n
                        if((it + 1) != strFile.end() && *(it+1) == 'n')
                        {
                            byChar = '\n';
                            //Skip the 'n' character
                            ++it;
                        }
                    }
                    strTextLine += byChar;
                }
                else 
                {
                    mpText[udwCurrentLine++] = UTF8String(strTextLine);
                    strTextLine.clear();
                }
            }
            
            return true;
        }
        
        //----------------------------------------------------------------------------
        /// Load Text ID
        ///
        /// Load the text ids from the external file
        /// @param File object for the text ids file
        /// @return Success
        //----------------------------------------------------------------------------
        bool CLocalisedText::LoadTextID(FileStreamSPtr& incIDFile)
        {
			if(incIDFile == nullptr || incIDFile->IsBad())
			{
				CS_WARNING_LOG("LocalisedTextLoader ID lookups unavailable: TagText.id missing");
				
				if(incIDFile->IsOpen())
                {
					incIDFile->Close();
                }
                
                return false;
			}

            std::string strID;
            
            mpTextLookup = new IDToLookupIndex(mudwLineCount);
            
            u32 udwCurrentLine = 0;
            while(udwCurrentLine < mudwLineCount)
            {
                incIDFile->GetLine(strID);
                mpTextLookup->Insert(strID, (LocalisedTextKey)udwCurrentLine);
                udwCurrentLine++;
            }
            
            incIDFile->Close();
            
            return true;
        }
	}
}
