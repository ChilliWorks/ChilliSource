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

#include <ChilliSource/Rendering/Sprite/SpriteSheetLoader.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Base/Screen.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>

namespace ChilliSource
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
		bool SpriteSheetLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		///
		/// @param Type to compare
		/// @return Whether the object can create a resource of given type
		//----------------------------------------------------------------------------
		bool SpriteSheetLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == SpriteSheet::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		///
		/// @param Type to compare
		/// @param Extension to compare
		/// @return Whether the object can create a resource with the given extension
		//----------------------------------------------------------------------------
		bool SpriteSheetLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
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
		bool SpriteSheetLoader::CreateResourceFromFile(Core::StorageLocation ineLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
			ChilliSource::Core::FileStreamSPtr binary_file = Core::Application::GetFileSystemPtr()->CreateFileStream(ineLocation, inFilePath, ChilliSource::Core::FileMode::k_readBinary);
			
			//If we have not successfully loaded the high res then just load the default
			if(binary_file->IsOpen() == false)
			{
				return false;
			}
			
			//Read the first four bytes to get the number of sprites. Remeber to flip the endianness
			s16 NumSprites = 0;
			binary_file->Read(reinterpret_cast<s8*>(&NumSprites), sizeof(s16));
			NumSprites = Core::Utils::Endian2ByteSwap(&NumSprites);
			
			//Read the header 2 bytes in but just discard it
			s16 wBinVersion = 0;
			binary_file->Read(reinterpret_cast<s8*>(&wBinVersion), sizeof(s16));
            wBinVersion = Core::Utils::Endian2ByteSwap(&wBinVersion);
            
            s16 wSpriteSheetWidth = 0;
            s16 wSpriteSheetHeight = 0;
            
            u32 udwNumElementsPerSprite = kNumElementsPerSpriteV0;
            if(wBinVersion >= 1)
            {
                udwNumElementsPerSprite = kNumElementsPerSpriteV1;
                
                if(wBinVersion >=2)
                {
                    binary_file->Read(reinterpret_cast<s8*>(&wSpriteSheetWidth), sizeof(s16));
                    wSpriteSheetWidth = Core::Utils::Endian2ByteSwap(&wSpriteSheetWidth);
                    
                    binary_file->Read(reinterpret_cast<s8*>(&wSpriteSheetHeight), sizeof(s16));
                    wSpriteSheetHeight = Core::Utils::Endian2ByteSwap(&wSpriteSheetHeight);
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
				SBuffer[i] = Core::Utils::Endian2ByteSwap(&SBuffer[i]);
			}
			
			//Now copy the data into our sprite data buffer as it is now in the correct format
			SpriteSheet* pSpriteSheet = (SpriteSheet*)(outpResource.get());
            pSpriteSheet->SetSpriteSheetSize(wSpriteSheetWidth, wSpriteSheetHeight);
			pSpriteSheet->SetNumSpriteFrames(NumSprites);
			
            s16 *pFrame = SBuffer;
			for(s32 i = 0; i < NumSprites; ++i)
			{
				SpriteSheet::SpriteFrame Frame;
				
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
			
			CS_SAFEDELETE_ARRAY(SBuffer);
            
            // Load in string IDs
            std::string strName;
            std::string strExtension;
            
            //Get the name of the file and append the high res identifier to it
            Core::StringUtils::SplitBaseFilename(inFilePath, strName, strExtension);
            Core::FileStreamSPtr idFile = Core::Application::GetFileSystemPtr()->CreateFileStream(ineLocation, strName + ".mospriteid", Core::FileMode::k_read);
            
            std::vector<u32> IDLookup;
            std::vector<std::string> IDStringLookup;
			if(idFile == nullptr || idFile->IsBad())
			{
				CS_LOG_WARNING("Sprite loader ID lookups unavailable: .mospriteid missing");
				
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
                        IDLookup.push_back(Core::HashCRC32::GenerateHashCode(strID));
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

