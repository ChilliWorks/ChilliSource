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
#include <ChilliSource/Core/File/CSBinaryChunk.h>
#include <ChilliSource/Core/File/CSBinaryInputStream.h>
#include <ChilliSource/Core/Resource/ResourcePool.h>
#include <ChilliSource/Core/String/UTF8StringUtils.h>
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
            const u32 k_fileFormatId = 1;
            const u32 k_fileFormatVersion = 2;
            //----------------------------------------------------------------------------
            /// Reads the contents of the INFO chunk in a CSFont file. This contains
            /// information relating to the source font, and data that applies to all
            /// glyphs.
            ///
            /// @author Ian Copland
            ///
            /// @param The INFO binary chunk.
            /// @param [Out] The font describing containing the output info.
            ///
            /// @return Whether or not the read was successful.
            //----------------------------------------------------------------------------
            bool ReadINFOChunk(Core::CSBinaryChunk* in_chunk, Font::Descriptor& out_desc)
            {
                const u32 k_infoChunkSize = 7 * 4;
                
                CS_ASSERT(in_chunk != nullptr, "INFO chunk cannot be null.");
                CS_ASSERT(in_chunk->GetSize() == k_infoChunkSize, "INFO chunk is the wrong size.");
                
                if (in_chunk != nullptr && in_chunk->GetSize() == k_infoChunkSize)
                {
                    out_desc.m_pointSize = in_chunk->Read<u32>();
                    out_desc.m_lineHeight = in_chunk->Read<u32>();
                    out_desc.m_descent = in_chunk->Read<u32>();
                    out_desc.m_spaceAdvance = in_chunk->Read<u32>();
                    
                    out_desc.m_textureAtlasWidth = in_chunk->Read<u32>();
                    out_desc.m_textureAtlasHeight = in_chunk->Read<u32>();
                    out_desc.m_verticalPadding = in_chunk->Read<u32>();
                    
                    return true;
                }
                
                return false;
            }
            //----------------------------------------------------------------------------
            /// Reads the contents of the CHAR chunk in a CSFont file. This contains the
            /// list of glyphs that the font can display.
            ///
            /// @author Ian Copland
            ///
            /// @param The CHAR binary chunk.
            /// @param [Out] The font describing containing the output info.
            ///
            /// @return Whether or not the read was successful.
            //----------------------------------------------------------------------------
            bool ReadCHARChunk(Core::CSBinaryChunk* in_chunk, Font::Descriptor& out_desc)
            {
                CS_ASSERT(in_chunk != nullptr, "CHAR chunk cannot be null.");
                
                if (in_chunk != nullptr)
                {
                    const u8* characterData = in_chunk->Read(in_chunk->GetSize());
                    out_desc.m_supportedCharacters = std::string(reinterpret_cast<const s8*>(characterData), in_chunk->GetSize());
                    return true;
                }
                
                return false;
            }
            //----------------------------------------------------------------------------
            /// Reads the contents of the GLPH chunk in a CSFont file. This contains the
            /// glyph data.
            ///
            /// @author Ian Copland
            ///
            /// @param The GLPH binary chunk.
            /// @param [Out] The font describing containing the output info.
            ///
            /// @return Whether or not the read was successful.
            //----------------------------------------------------------------------------
            bool ReadGLPHChunk(Core::CSBinaryChunk* in_chunk, Font::Descriptor& out_desc)
            {
                const u32 k_glyphInfoSize = 10 * 2;
                
                CS_ASSERT(in_chunk != nullptr, "GLPH chunk cannot be null.");
                CS_ASSERT(in_chunk->GetSize() % k_glyphInfoSize == 0, "GLPH chunk is the wrong size.");
                
                if (in_chunk != nullptr && in_chunk->GetSize() % k_glyphInfoSize == 0)
                {
                    const u32 numGlyphs = in_chunk->GetSize() / k_glyphInfoSize;
                    for (u32 i = 0; i < numGlyphs; ++i)
                    {
                        Font::Frame frame;
                        frame.m_texCoordU = in_chunk->Read<s16>();
                        frame.m_texCoordV = in_chunk->Read<s16>();
                        frame.m_width = in_chunk->Read<s16>();
                        frame.m_height = in_chunk->Read<s16>();
                        frame.m_offsetX = in_chunk->Read<s16>();
                        frame.m_offsetY = in_chunk->Read<s16>();
                        frame.m_originalWidth = in_chunk->Read<s16>();
                        frame.m_originalHeight = in_chunk->Read<s16>();
                        frame.m_origin = in_chunk->Read<s16>();
                        frame.m_advance = in_chunk->Read<s16>();
                        out_desc.m_frames.push_back(frame);
                    }
                    
                    return true;
                }
                
                return false;
            }
            //----------------------------------------------------------------------------
            /// Loads the contents of a CSFont file into the given font description.
            ///
            /// @author Ian Copland
            ///
            /// @param The storage location of the file.
            /// @param The file path.
            /// @param [Out] Font description populated by this function
            ///
            /// @return Whether or not the read was successful.
            //----------------------------------------------------------------------------
            bool LoadCSFont(Core::StorageLocation in_storageLocation, const std::string& in_filePath, Font::Descriptor& out_desc)
            {
                //create the stream.
                Core::CSBinaryInputStream stream(in_storageLocation, in_filePath);
                if (stream.IsValid() == false || stream.GetFileFormatId() != k_fileFormatId || stream.GetFileFormatVersion() != k_fileFormatVersion)
                {
                    CS_LOG_ERROR("Could not open csfont file: " + in_filePath);
                    return false;
                }
                
                //Read the INFO chunk.
                Core::CSBinaryChunkUPtr chunk = stream.ReadChunk("INFO");
                if (chunk == nullptr || ReadINFOChunk(chunk.get(), out_desc) == false)
                {
                    CS_LOG_ERROR("Could not read INFO chunk in csfont file: " + in_filePath);
                    return false;
                }
                
                
                //Read the CHAR chunk.
                chunk = stream.ReadChunk("CHAR");
                if (chunk == nullptr || ReadCHARChunk(chunk.get(), out_desc) == false)
                {
                    CS_LOG_ERROR("Could not read CHAR chunk in csfont file: " + in_filePath);
                    return false;
                }
                
                //Read the GLPH chunk.
                chunk = stream.ReadChunk("GLPH");
                if (chunk == nullptr || ReadGLPHChunk(chunk.get(), out_desc) == false)
                {
                    CS_LOG_ERROR("Could not read GLPH chunk in csfont file: " + in_filePath);
                    return false;
                }
                
                //ensure the data makes sense
                u32 numCharacters = Core::UTF8StringUtils::CalcLength(out_desc.m_supportedCharacters.begin(), out_desc.m_supportedCharacters.end());
                if (out_desc.m_frames.size() != numCharacters)
                {
                    CS_LOG_ERROR("Glyph count different to character count in font: " + in_filePath);
                    return false;
                }
                
                return true;
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
			std::string fileName, fileExtension;
            Core::StringUtils::SplitBaseFilename(in_filePath, fileName, fileExtension);
            
            const std::string textureFilePath(fileName + "." + k_textureFileExtension);
            
            if(in_delegate == nullptr)
            {
                Font::Descriptor desc;
                desc.m_texture = Core::Application::Get()->GetResourcePool()->LoadResource<Texture>(in_location, textureFilePath);
                if(desc.m_texture == nullptr)
                {
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                    return;
                }
                
                if (LoadCSFont(in_location, in_filePath, desc) == false)
                {
                    out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                    return;
                }
                
                Font* font = (Font*)(out_resource.get());
                font->Build(desc);
                out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
            }
            else
            {
                Core::Application::Get()->GetResourcePool()->LoadResourceAsync<Texture>(in_location, textureFilePath, [out_resource, in_delegate, in_location, in_filePath](const TextureCSPtr& in_texture)
                {
                    if(in_texture != nullptr)
                    {
                        Core::Application::Get()->GetTaskScheduler()->ScheduleTask([out_resource, in_delegate, in_location, in_filePath, in_texture]()
                        {
                            Font::Descriptor desc;
                            desc.m_texture = in_texture;
                            
                            if (LoadCSFont(in_location, in_filePath, desc) == true)
                            {
                                Font* font = (Font*)(out_resource.get());
                                font->Build(desc);
                                out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
                            }
                            else
                            {
                                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                            }

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
