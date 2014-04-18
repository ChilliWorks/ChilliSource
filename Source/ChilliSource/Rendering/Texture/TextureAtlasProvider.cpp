//
//  TextureAtlasProvider.cpp
//  Chilli Source
//
//  Created by Scott Downie on 22/10/2010.
//  Copyright 2010 Tag Games. All rights reserved.
//

#include <ChilliSource/Rendering/Texture/TextureAtlasProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Cryptographic/HashCRC32.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_framesFileExtension("bin");
            const std::string k_keysFileExtension("mospriteid");
            
            const u32 k_numElementsPerFrame = 8;
        }
        
        CS_DEFINE_NAMEDTYPE(TextureAtlasProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        TextureAtlasProviderUPtr TextureAtlasProvider::Create()
        {
            return TextureAtlasProviderUPtr(new TextureAtlasProvider());
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool TextureAtlasProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == TextureAtlasProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        Core::InterfaceIDType TextureAtlasProvider::GetResourceType() const
        {
            return TextureAtlas::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool TextureAtlasProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_framesFileExtension;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void TextureAtlasProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
		{
            LoadResource(in_location, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void TextureAtlasProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource)
        {
            Core::Task<Core::StorageLocation, const std::string&, const Core::ResourceProvider::AsyncLoadDelegate&, Core::ResourceSPtr&>
            task(this, &TextureAtlasProvider::LoadResource, in_location, in_filePath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void TextureAtlasProvider::LoadResource(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource)
        {
            TextureAtlas* spriteResource(static_cast<TextureAtlas*>(out_resource.get()));
            
            TextureAtlas::Descriptor desc;
            LoadFrames(in_location, in_filePath, desc);
            LoadMap(in_location, in_filePath, desc);
            
            //TODO: We cannot add the guarantee that the map file is loaded as fonts use this path and don't require a map file. This will change.
            if(desc.m_frames.size() > 0 /*&& desc.m_keys.size() > 0*/)
            {
                spriteResource->SetLoadState(Core::Resource::LoadState::k_loaded);
                spriteResource->Build(desc);
            }
            else
            {
                spriteResource->SetLoadState(Core::Resource::LoadState::k_failed);
            }
            
            if(in_delegate != nullptr)
            {
                Core::Task<Core::ResourceSPtr&> task(in_delegate, out_resource);
                Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
            }
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void TextureAtlasProvider::LoadFrames(Core::StorageLocation in_location, const std::string& in_filePath, TextureAtlas::Descriptor& out_desc)
        {
            ChilliSource::Core::FileStreamSPtr frameFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, ChilliSource::Core::FileMode::k_readBinary);
			
			if(frameFile->IsOpen() == false)
			{
				return;
			}
			
			s16 numFrames = 0;
			frameFile->Read(reinterpret_cast<s8*>(&numFrames), sizeof(s16));
			numFrames = Core::Utils::Endian2ByteSwap(&numFrames);
			
			s16 binVersion = 0;
			frameFile->Read(reinterpret_cast<s8*>(&binVersion), sizeof(s16));
            binVersion = Core::Utils::Endian2ByteSwap(&binVersion);
            
            CS_ASSERT(binVersion >= 2, "TextureAtlas minimum version supported is 2.0");
            
            s16 textureAtlasWidth = 0;
            frameFile->Read(reinterpret_cast<s8*>(&textureAtlasWidth), sizeof(s16));
            textureAtlasWidth = Core::Utils::Endian2ByteSwap(&textureAtlasWidth);
            
            s16 textureAtlasHeight = 0;
            frameFile->Read(reinterpret_cast<s8*>(&textureAtlasHeight), sizeof(s16));
            textureAtlasHeight = Core::Utils::Endian2ByteSwap(&textureAtlasHeight);
			
			//Temporary buffer to hold our unformatted data
			const u32 numElements = numFrames * k_numElementsPerFrame;
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
            out_desc.m_textureAtlasWidth = (u32)textureAtlasWidth;
            out_desc.m_textureAtlasHeight = (u32)textureAtlasHeight;
            out_desc.m_frames.reserve(numFrames);
            
            s16* framePtr = buffer;
			for(u32 i=0; i<(u32)numFrames; ++i)
			{
				TextureAtlas::Frame frame;
				
				frame.m_texCoordU = framePtr[0];
				frame.m_texCoordV = framePtr[1];
				frame.m_width = framePtr[2];
				frame.m_height = framePtr[3];
				frame.m_offsetX = framePtr[4];
				frame.m_offsetY = framePtr[5];
                frame.m_originalWidth = framePtr[6];
                frame.m_originalHeight = framePtr[7];
                
				out_desc.m_frames.push_back(frame);
                framePtr += k_numElementsPerFrame;
			}
			
			CS_SAFEDELETE_ARRAY(buffer);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void TextureAtlasProvider::LoadMap(Core::StorageLocation in_location, const std::string& in_filePath, TextureAtlas::Descriptor& out_desc)
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
                
                std::string spriteID;
				while(!mapFile->EndOfFile())
				{
                    mapFile->GetLine(spriteID);
                    if(spriteID.empty() == false)
                    {
                        out_desc.m_keys.push_back(Core::HashCRC32::GenerateHashCode(spriteID));
                        spriteID.clear();
                    }
				}
			}
        }
	}
}

