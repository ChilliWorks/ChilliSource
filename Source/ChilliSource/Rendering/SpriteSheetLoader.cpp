/*
 * File: SpriteSheetLoader.cpp
 * Date: 22/10/2010 2010 
 * Description: 
 */

/*
 * Author: Scott Downie
 * Version: v 1.0
 * Copyright ©2010 Tag Games Limited - All rights reserved 
 */

#include <ChilliSource/Rendering/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/SpriteSheet.h>

#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Core/Utils.h>
#include <ChilliSource/Core/Screen.h>
#include <ChilliSource/Core/HashCRC32.h>

namespace moFlo
{
	namespace Rendering
	{
		const std::string kstrTagSpriteExtension("bin");
		
        // Specify for elements bin sprite versions
        const u32 kNumElementsPerSpriteV0 = 6;
        const u32 kNumElementsPerSpriteV1 = 8;
		
		//-------------------------------------------------------------------------
		/// Is A
		///
		/// @param Interface to compare
		/// @return Whether the object implements the given interface
		//-------------------------------------------------------------------------
		bool CSpriteSheetLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool CSpriteSheetLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Rendering::CSpriteSheet::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool CSpriteSheetLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrTagSpriteExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		///
		/// @param Location to load from
		/// @param Filename
		/// @param Out: Resource object
		/// @return Whether the resource loaded
		//----------------------------------------------------------------------------
		bool CSpriteSheetLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			moFlo::Core::FileStreamPtr binary_file = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineLocation, inFilePath, moFlo::Core::FM_READ_BINARY);
			
			//If we have not successfully loaded the high res then just load the default
			if(binary_file->IsOpen() == false)
			{
				return false;
			}
			
			//Read the first four bytes to get the number of sprites. Remeber to flip the endianness
			s16 NumSprites = 0;
			binary_file->Read(reinterpret_cast<s8*>(&NumSprites), sizeof(s16));
			NumSprites = BYTE_SWAP_2(&NumSprites);
			
			//Read the header 2 bytes in but just discard it
			s16 wBinVersion = 0;
			binary_file->Read(reinterpret_cast<s8*>(&wBinVersion), sizeof(s16));
            wBinVersion = BYTE_SWAP_2(&wBinVersion);
            
            s16 wSpriteSheetWidth = 0;
            s16 wSpriteSheetHeight = 0;
            
            u32 udwNumElementsPerSprite = kNumElementsPerSpriteV0;
            if(wBinVersion >= 1)
            {
                udwNumElementsPerSprite = kNumElementsPerSpriteV1;
                
                if(wBinVersion >=2)
                {
                    binary_file->Read(reinterpret_cast<s8*>(&wSpriteSheetWidth), sizeof(s16));
                    wSpriteSheetWidth = BYTE_SWAP_2(&wSpriteSheetWidth);
                    
                    binary_file->Read(reinterpret_cast<s8*>(&wSpriteSheetHeight), sizeof(s16));
                    wSpriteSheetHeight = BYTE_SWAP_2(&wSpriteSheetHeight);
                }
            }
            
			
			//Temporary buffer to hold our unformatted data
			const u16 NumElements = NumSprites * udwNumElementsPerSprite; 
			s16* SBuffer = new s16[NumElements];

			//Fetch the binary data in one read.
			binary_file->Read(reinterpret_cast<s8*>(SBuffer), NumElements * sizeof(s16));
			binary_file->Close();
			
			//Swap for endianess as the tool does it backwards!
			for(u32 i = 0; i<NumElements; ++i)
			{
				SBuffer[i] = BYTE_SWAP_2(&SBuffer[i]);
			}
			
			//Now copy the data into our sprite data buffer as it is now in the correct format
			CSpriteSheet* pSpriteSheet = (CSpriteSheet*)(outpResource.get());
            pSpriteSheet->SetSpriteSheetSize(wSpriteSheetWidth, wSpriteSheetHeight);
			pSpriteSheet->SetNumSpriteFrames(NumSprites);
			
            s16 *pFrame = SBuffer;
			for(s32 i = 0; i < NumSprites; ++i)
			{
				CSpriteSheet::SpriteFrame Frame;
				
				Frame.U			= pFrame[0];
				Frame.V			= pFrame[1];
				Frame.Width		= pFrame[2];
				Frame.Height	= pFrame[3];
				Frame.OffsetX	= pFrame[4];
				Frame.OffsetY	= pFrame[5];
                Frame.OriginalWidth = 0;
                Frame.OriginalHeight = 0;
                
                if(wBinVersion >=1)
                {
                    Frame.OriginalWidth = pFrame[6];
                    Frame.OriginalHeight = pFrame[7];
                }               

				pSpriteSheet->AddSpriteFrame(Frame);
                pFrame += udwNumElementsPerSprite;
			}
			
			SAFE_DELETE_ARRAY(SBuffer);
            
            // Load in string IDs
            std::string strName;
            std::string strExtension;
            
            //Get the name of the file and append the high res identifier to it
            Core::CStringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
            Core::FileStreamPtr idFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineLocation, strName + ".mospriteid", Core::FM_READ);
            
            DYNAMIC_ARRAY<u32> IDLookup;
            DYNAMIC_ARRAY<std::string> IDStringLookup;
			if(idFile == NULL || idFile->IsBad())
			{
				WARNING_LOG("Sprite loader ID lookups unavailable: .mospriteid missing");
				
				if(idFile->IsOpen())
					idFile->Close();
			}
			else 
			{
                std::string strID;
				while(!idFile->EndOfFile())
				{
                    s8 byNextChar = 0;
                    idFile->Get(byNextChar);
                    
                    if(byNextChar != '\n')
                    {
                    	if(byNextChar != '\r')
                    		strID += byNextChar;
                    }
                    else
                    {
                        IDLookup.push_back(CHashCRC32::GenerateHashCode(strID));
                        IDStringLookup.push_back(strID);
                        strID.clear();
                    }					
				}
			}
			
            pSpriteSheet->SetIDLookups(IDLookup, IDStringLookup);
			return true;
		}
	}
}

