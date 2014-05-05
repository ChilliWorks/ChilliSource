//
//  LocalisedTextProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 25/04/2014.
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

#include <ChilliSource/Core/Localisation/LocalisedTextProvider.h>

#include <ChilliSource/Core/Localisation/LocalisedText.h>

namespace ChilliSource
{
	namespace Core
	{
		namespace
		{
			const std::string k_fileExtension("mofloloca");

			//----------------------------------------------------
			/// Performs the heavy lifting for the 2 create methods
			/// by loading the keys and text files into a single resource
			///
			/// @author S Downie
			///
			/// @param The storage location.
			/// @param The filepath.
			/// @param Completion delegate
			/// @param [Out] The output resource.
			//----------------------------------------------------
			void CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
			{
				mudwLineCount = 0;

				FileStreamSPtr localFile = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, FileMode::k_read);
				// Load localised text
				if (LoadLocalisedText(localFile) == false)
				{
					return false;
				}

				FileStreamSPtr idFile = Application::Get()->GetFileSystem()->CreateFileStream(ineLocation, inFilePath + "TagText.id", FileMode::k_read);
				// Load in string IDs
				if (LoadTextID(idFile) == false)
				{
					return false;
				}

				return true;
			}
			bool LocalisedText::LoadLocalisedText(FileStreamSPtr& incLocaleFile)
			{
				//if bad, then bail!
				if (incLocaleFile == nullptr || incLocaleFile->IsBad() == true)
				{
					if (incLocaleFile->IsOpen())
					{
						incLocaleFile->Close();
					}

					return false;
				}

				// Check we have not seeked beyond file
				if (incLocaleFile->IsBad() == true)
				{
					if (incLocaleFile->IsOpen())
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
				for (it = strFile.begin(); it != strFile.end(); ++it)
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

					if (bNewLine == true)
					{
						udwLineCount++;
						udwCurrentLine = 0;
					}
				}

				// Catch if last line has no newline
				if (udwCurrentLine != 0)
				{
					udwLineCount++;
				}

				mudwLineCount = udwLineCount;
				mpText = new UTF8String[mudwLineCount];

				std::string strTextLine;
				udwCurrentLine = 0;
				for (it = strFile.begin(); it != strFile.end(); ++it)
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
						if (byChar == '\\')
						{
							//Peek ahead for an n
							if ((it + 1) != strFile.end() && *(it + 1) == 'n')
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

			bool LocalisedText::LoadTextID(FileStreamSPtr& incIDFile)
			{
				if (incIDFile == nullptr || incIDFile->IsBad())
				{
					CS_LOG_WARNING("LocalisedTextLoader ID lookups unavailable: TagText.id missing");

					if (incIDFile->IsOpen())
					{
						incIDFile->Close();
					}

					return false;
				}

				std::string strID;

				mpTextLookup = new IDToLookupIndex(mudwLineCount, MakeDelegate<u32, const std::string&>(HashCRC32::GenerateHashCode));

				u32 udwCurrentLine = 0;
				while (udwCurrentLine < mudwLineCount)
				{
					incIDFile->GetLine(strID);
					mpTextLookup->insert(strID, (LocalisedTextKey)udwCurrentLine);
					udwCurrentLine++;
				}

				incIDFile->Close();

				return true;
			}
		}

		CS_DEFINE_NAMEDTYPE(LocalisedTextProvider);

		//-------------------------------------------------------
		//-------------------------------------------------------
		LocalisedTextProviderUPtr LocalisedTextProvider::Create()
		{
			return LocalisedTextProviderUPtr(new LocalisedTextProvider());
		}
		//----------------------------------------------------
		//----------------------------------------------------
		InterfaceIDType LocalisedTextProvider::GetResourceType() const
		{
			return LocalisedText::InterfaceID;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		bool LocalisedTextProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_fileExtension;
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void LocalisedTextProvider::CreateResourceFromFile(StorageLocation in_storageLocation, const std::string& in_filePath, const ResourceSPtr& out_resource)
		{

		}
		//----------------------------------------------------
		//----------------------------------------------------
		void LocalisedTextProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
		{

		}
	}
}
