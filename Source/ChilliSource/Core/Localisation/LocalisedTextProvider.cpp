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

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Core/Localisation/LocalisedText.h>

namespace ChilliSource
{
	namespace Core
	{
		namespace
		{
			const std::string k_fileExtension("mofloloca");

            //----------------------------------------------------
			/// Loads the localised text file and parses it into
            /// individual text lines. Replaces any new-line characters
            /// with a standardised '\n'
			///
			/// @author S Downie
			///
			/// @param Valid file stream
			/// @param [Out] The text split into lines
			//----------------------------------------------------
            void LoadLocalisedText(FileStreamSPtr& in_fileStream, std::vector<UTF8String>& out_text)
			{
				// Read file
				std::string fileBuffer;
				in_fileStream->GetAll(fileBuffer);
				in_fileStream->Close();
                
				std::string strTextLine;
	
				for (auto it = fileBuffer.begin(); it != fileBuffer.end(); ++it)
				{
					u8 byChar = *it;
					bool bNewLine = false;
                    
					//insure that windows evil "\n\r" doesnt cause any problems
					if (byChar == '\n')
					{
						auto itNext = it + 1;
						if (itNext != fileBuffer.end())
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
						auto itNext = it + 1;
						if (itNext != fileBuffer.end())
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
							if ((it + 1) != fileBuffer.end() && *(it + 1) == 'n')
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
                        out_text.push_back(UTF8String(strTextLine));
						strTextLine.clear();
					}
				}
			}
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
			void LoadResource(StorageLocation in_storageLocation, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
			{
                LocalisedText* textResource((LocalisedText*)out_resource.get());

                //---Load the file that contains the text
				FileStreamSPtr textFile = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, in_filePath, FileMode::k_read);
                if (textFile == nullptr || textFile->IsBad() == true)
				{
                    CS_LOG_ERROR("Failed to load localised text due to missing file: " + in_filePath);
                    textResource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(in_delegate, out_resource));
                    }
					return;
				}
                
                std::vector<UTF8String> values;
				LoadLocalisedText(textFile, values);

                //---Load the file that contains the keys from the same directory. (TODO: Turn this into a single resource i.e. a zip file containing the keys and texts)
                std::string fileName;
                std::string fileExtension;
                std::string basePath;
                Core::StringUtils::SplitFullFilename(in_filePath, fileName, fileExtension, basePath);
                
				FileStreamSPtr keyFile = Application::Get()->GetFileSystem()->CreateFileStream(in_storageLocation, basePath + "TagText.id", FileMode::k_read);
                if (keyFile == nullptr || keyFile->IsBad())
				{
                    CS_LOG_ERROR("Failed to load localised text due to missing TagText.id file");
                    textResource->SetLoadState(Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
                        Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(in_delegate, out_resource));
                    }
					return;
				}
                
                std::vector<std::string> keys;
                std::string key;
                while(keyFile->EndOfFile() == false)
                {
                    keyFile->GetLine(key);
                    keys.push_back(std::move(key));
                }
                
                textResource->Build(keys, values);
                textResource->SetLoadState(Resource::LoadState::k_loaded);
                
                if(in_delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(in_delegate, out_resource));
                }
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
        bool LocalisedTextProvider::IsA(InterfaceIDType in_interfaceId) const
        {
            return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == LocalisedTextProvider::InterfaceID;
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
            LoadResource(in_storageLocation, in_filePath, nullptr, out_resource);
		}
		//----------------------------------------------------
		//----------------------------------------------------
		void LocalisedTextProvider::CreateResourceFromFileAsync(StorageLocation in_storageLocation, const std::string& in_filePath, const AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
		{
            Application::Get()->GetTaskScheduler()->ScheduleTask(std::bind(LoadResource, in_storageLocation, in_filePath, in_delegate, out_resource));
		}
	}
}
