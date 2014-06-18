//
//  FontProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 26/10/2010.
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

#include <ChilliSource/Rendering/Font/FontProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/Utils.h>
#include <ChilliSource/Core/Resource/Resourcepool.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Font/Font.h>
#include <ChilliSource/Rendering/Texture/Texture.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_fontFileExtension("csfont");
            const std::string k_textureFileExtension("csimage");
            
            const u32 k_numElementsPerFrame = 8;
            
            //----------------------------------------------------------------------------
            /// Load the frames for the characters that represent this font
            ///
            /// @author S Downie
            ///
            /// @param File stream
            ///
            /// @param [Out] Font description populated by this function
            //----------------------------------------------------------------------------
            void LoadFrames(const Core::FileStreamSPtr& in_fileStream, Font::Descriptor& out_desc)
            {
                s16 numFrames = 0;
                in_fileStream->Read(reinterpret_cast<s8*>(&numFrames), sizeof(s16));
                
                s16 binVersion = 0;
                in_fileStream->Read(reinterpret_cast<s8*>(&binVersion), sizeof(s16));
                
                s16 textureAtlasWidth = 0;
                in_fileStream->Read(reinterpret_cast<s8*>(&textureAtlasWidth), sizeof(s16));
                
                s16 textureAtlasHeight = 0;
                in_fileStream->Read(reinterpret_cast<s8*>(&textureAtlasHeight), sizeof(s16));
                
                s16 lineHeight = 0;
                in_fileStream->Read(reinterpret_cast<s8*>(&lineHeight), sizeof(s16));
                
                //Temporary buffer to hold our unformatted data
                const u32 numElements = numFrames * k_numElementsPerFrame;
                s16* buffer = new s16[numElements];
                
                //Fetch the binary data in one read.
                in_fileStream->Read(reinterpret_cast<s8*>(buffer), numElements * sizeof(s16));
                
                //Read the supported characters which are UTF-8 encoded
				s8 character;
				in_fileStream->Read(&character, sizeof(s8));

				while (character != '\0')
                {
					out_desc.m_supportedCharacters += character;
                    in_fileStream->Read(&character, sizeof(s8));
                }
                
                in_fileStream->Close();
                
                //Now copy the data into our sprite data buffer as it is now in the correct format
                out_desc.m_textureAtlasWidth = (u32)textureAtlasWidth;
                out_desc.m_textureAtlasHeight = (u32)textureAtlasHeight;
                out_desc.m_lineHeight = (u32)lineHeight;
                out_desc.m_frames.reserve(numFrames);
                
                s16* framePtr = buffer;
                for(s16 i=0; i<numFrames; ++i)
                {
                    Font::Frame frame;
                    
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
        }
		
        CS_DEFINE_NAMEDTYPE(FontProvider);
        
        //-------------------------------------------------------
        //-------------------------------------------------------
        FontProviderUPtr FontProvider::Create()
        {
            return FontProviderUPtr(new FontProvider());
        }
		//-------------------------------------------------------------------------e
		//-------------------------------------------------------------------------
		bool FontProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == FontProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        Core::InterfaceIDType FontProvider::GetResourceType() const
        {
            return Font::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool FontProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_fontFileExtension;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void FontProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
            LoadFont(in_location, in_filePath, nullptr, out_resource);
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void FontProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			auto task = std::bind(&FontProvider::LoadFont, this, in_location, in_filePath, in_delegate, out_resource);
            Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void FontProvider::LoadFont(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
        {
			Core::FileStreamSPtr fontStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_readBinary);
            if(fontStream == nullptr || fontStream->IsBad())
            {
                CS_LOG_ERROR("Failed to open font file: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
            std::string fileName, fileExtension;
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            
            const std::string textureFilePath(fileName + "." + k_textureFileExtension);
            
            if(in_delegate == nullptr)
            {
                Font::Descriptor desc;
                desc.m_texture = Core::Application::Get()->GetResourcePool()->LoadResource<Texture>(in_location, textureFilePath);
                if(desc.m_texture != nullptr)
                {
                    LoadFrames(fontStream, desc);
                    Font* font = (Font*)(out_resource.get());
                    font->Build(desc);
                    out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
                }
                else
                {
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                }
            }
            else
            {
                Core::Application::Get()->GetResourcePool()->LoadResourceAsync<Texture>(in_location, textureFilePath, [out_resource, in_delegate, fontStream](const TextureCSPtr& in_texture)
                {
                    if(in_texture != nullptr)
                    {
                        Core::Application::Get()->GetTaskScheduler()->ScheduleTask([out_resource, in_delegate, fontStream, in_texture]()
                        {
                            Font::Descriptor desc;
                            LoadFrames(fontStream, desc);
                            desc.m_texture = in_texture;
                            Font* font = (Font*)(out_resource.get());
                            font->Build(desc);
                            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
                            Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                        });
                    }
                    else
                    {
                        //Already on main thread
                        out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                        in_delegate(out_resource);
                    }
                });
            }
        }
	}
}
