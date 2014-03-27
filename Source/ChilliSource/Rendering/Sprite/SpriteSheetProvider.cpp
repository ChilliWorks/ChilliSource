//
//  SpriteSheetProvider.cpp
//  Chilli Source
//
//  Created by Scott Downie on 22/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Sprite/SpriteSheetProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Sprite/SpriteSheet.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_spriteFramesExtension("bin");
            const std::string k_spriteMapExtension("mospriteid");
            
            // Specify for elements bin sprite versions
            const u32 k_numElementsPerSpriteV0 = 6;
            const u32 k_numElementsPerSpriteV1 = 8;
        }
        
        CS_DEFINE_NAMEDTYPE(SpriteSheetProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        SpriteSheetProviderUPtr SpriteSheetProvider::Create()
        {
            return SpriteSheetProviderUPtr(new SpriteSheetProvider());
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool SpriteSheetProvider::IsA(Core::InterfaceIDType in_interfaceID) const
		{
			return in_interfaceID == ResourceProvider::InterfaceID || in_interfaceID == SpriteSheetProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool SpriteSheetProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceID) const
		{
			return (in_interfaceID == SpriteSheet::InterfaceID);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool SpriteSheetProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return (in_extension == k_spriteFramesExtension || in_extension == k_spriteMapExtension);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool SpriteSheetProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
		{
            SpriteSheetSPtr spriteResource(std::static_pointer_cast<SpriteSheet>(out_resource));
            spriteResource->SetLoaded(false);
            
            LoadFrames(in_location, in_filePath, spriteResource);
            LoadMap(in_location, in_filePath, spriteResource);
            
            //TODO: We cannot guarantee that the map file is loaded as fonts use this path and don't require a map file
            spriteResource->SetLoaded(spriteResource->GetNumSpriteFrames() > 0);// && spriteResource->GetNumSpriteLookupKeys() > 0);
            
            return spriteResource->IsLoaded();
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        bool SpriteSheetProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
        {
            out_resource->SetLoaded(false);
            
            SpriteSheetSPtr spriteResource(std::static_pointer_cast<SpriteSheet>(out_resource));
            
            CS_LOG_WARNING("Async SpriteSheet Loading Not Implemented");
            return false;
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void SpriteSheetProvider::LoadFrames(Core::StorageLocation in_location, const std::string& in_filePath, SpriteSheetSPtr& out_resource)
        {
            ChilliSource::Core::FileStreamSPtr frameFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, ChilliSource::Core::FileMode::k_readBinary);
			
			if(frameFile->IsOpen() == false)
			{
				return;
			}
			
			//Read the first four bytes to get the number of sprites. Remeber to flip the endianness
			s16 numSprites = 0;
			frameFile->Read(reinterpret_cast<s8*>(&numSprites), sizeof(s16));
			numSprites = Core::Utils::Endian2ByteSwap(&numSprites);
			
			//Read the header 2 bytes in but just discard it
			s16 binVersion = 0;
			frameFile->Read(reinterpret_cast<s8*>(&binVersion), sizeof(s16));
            binVersion = Core::Utils::Endian2ByteSwap(&binVersion);
            
            s16 spriteSheetWidth = 0;
            s16 spriteSheetHeight = 0;
            
            u32 numElementsPerSprite = k_numElementsPerSpriteV0;
            if(binVersion >= 1)
            {
                numElementsPerSprite = k_numElementsPerSpriteV1;
                
                if(binVersion >=2)
                {
                    frameFile->Read(reinterpret_cast<s8*>(&spriteSheetWidth), sizeof(s16));
                    spriteSheetWidth = Core::Utils::Endian2ByteSwap(&spriteSheetWidth);
                    
                    frameFile->Read(reinterpret_cast<s8*>(&spriteSheetHeight), sizeof(s16));
                    spriteSheetHeight = Core::Utils::Endian2ByteSwap(&spriteSheetHeight);
                }
            }
            
			
			//Temporary buffer to hold our unformatted data
			const u32 numElements = numSprites * numElementsPerSprite;
			s16* buffer = new s16[numElements];
            
			//Fetch the binary data in one read.
			frameFile->Read(reinterpret_cast<s8*>(buffer), numElements * sizeof(s16));
			frameFile->Close();
			
			//Swap for endianess as the tool does it backwards!
			for(u32 i=0; i<numElements; ++i)
			{
				buffer[i] = Core::Utils::Endian2ByteSwap(&buffer[i]);
			}
			
			//Now copy the data into our sprite data buffer as it is now in the correct format
			SpriteSheet* spriteSheet = (out_resource.get());
            spriteSheet->SetSpriteSheetSize(spriteSheetWidth, spriteSheetHeight);
			spriteSheet->SetNumSpriteFrames(numSprites);
			
            s16* framePtr = buffer;
			for(u32 i=0; i<(u32)numSprites; ++i)
			{
				SpriteSheet::SpriteFrame frame;
				
				frame.U	= framePtr[0];
				frame.V	= framePtr[1];
				frame.Width	= framePtr[2];
				frame.Height = framePtr[3];
				frame.OffsetX = framePtr[4];
				frame.OffsetY = framePtr[5];
                frame.OriginalWidth = 0;
                frame.OriginalHeight = 0;
                
                if(binVersion >= 1)
                {
                    frame.OriginalWidth = framePtr[6];
                    frame.OriginalHeight = framePtr[7];
                }
                
				spriteSheet->AddSpriteFrame(frame);
                framePtr += numElementsPerSprite;
			}
			
			CS_SAFEDELETE_ARRAY(buffer);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void SpriteSheetProvider::LoadMap(Core::StorageLocation in_location, const std::string& in_filePath, SpriteSheetSPtr& out_resource)
        {
            //The string IDs are loaded as a by-product so we have to deduce their file type
            std::string fileName;
            std::string fileExtension;
            
            //Get the name of the file and append the high res identifier to it
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            Core::FileStreamSPtr mapFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, fileName + ".mospriteid", Core::FileMode::k_read);
            
			if(mapFile->IsOpen())
			{
                std::vector<u32> IDHashedLookup;
                std::vector<std::string> IDStringLookup;
                
                std::string spriteID;
				while(!mapFile->EndOfFile())
				{
                    mapFile->GetLine(spriteID);
                    if(spriteID.empty() == false)
                    {
                        IDHashedLookup.push_back(Core::HashCRC32::GenerateHashCode(spriteID));
                        IDStringLookup.push_back(spriteID);
                        spriteID.clear();
                    }
				}
                
                out_resource->SetIDLookups(IDHashedLookup, IDStringLookup);
			}
        }
	}
}

