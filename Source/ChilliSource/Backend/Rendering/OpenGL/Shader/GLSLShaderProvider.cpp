//
//  GLSLShaderProvider.cpp
//  Chilli Source
//  Created by Scott Downie on 11/04/2014.
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

#include <ChilliSource/Backend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>

#include <ChilliSource/Backend/Rendering/OpenGL/Shader/Shader.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>

namespace ChilliSource
{
	namespace OpenGL
	{
        namespace
        {
            const std::string k_shaderFileExtension("csshader");
            const std::string k_languageTag("GLSL");
            const std::string k_vsTag("VertexShader");
            const std::string k_fsTag("FragmentShader");
            
            //----------------------------------------------
            /// Seeks through the given string from the
            /// given position to get the next "chunk". A
            /// chunk consists of text enclosed in braces
            /// with a title in the following format:
            ///
            ///		Title
            ///		{
            ///			Text
            ///		}
            ///
            /// If no chunk can be found, empty strings
            /// will be returned.
            ///
            /// @author I Copland
            ///
            /// @param The string to search in.
            /// @param The position in the string to start
            /// searching.
            /// @param [Out] The chunk name.
            /// @param [Out] The chunk contents.
            ///
            /// @return The next position to continue looking
            /// for chunks.
            //----------------------------------------------
            u32 GetNextChunk(const std::string& in_text, u32 in_startIndex, std::string& out_title, std::string& out_contents)
            {
                //find the first open brace
                u32 openBraceLocation = in_text.find("{", in_startIndex);
                if (openBraceLocation == in_text.npos)
                {
                    return in_text.length();
                }
                
                //iterate over to find the matching close brace
                u32 closeBraceLocation = openBraceLocation;
                u32 additionalOpenBraceCount = 0;
                for (u32 i = openBraceLocation + 1; i < in_text.size(); ++i)
                {
                    bool bContinue = true;
                    switch (in_text[i])
                    {
                        case '{':
                            ++additionalOpenBraceCount;
                            break;
                        case '}':
                            if (additionalOpenBraceCount <= 0)
                            {
                                closeBraceLocation = i;
                                bContinue = false;
                            }
                            else
                            {
                                --additionalOpenBraceCount;
                            }
                            break;
                        default:
                            break;
                    }
                    
                    if (bContinue == false)
                    {
                        break;
                    }
                }
                
                if (openBraceLocation == closeBraceLocation)
                {
                    return in_text.length();
                }
                
                //get the title and contents
                out_title = in_text.substr(in_startIndex, openBraceLocation - in_startIndex);
                out_contents = in_text.substr(openBraceLocation + 1, closeBraceLocation - (openBraceLocation + 1));
                
                return closeBraceLocation + 1;
            }
            //----------------------------------------------
            /// returns a new map containing the information
            /// on all chunks in the given text.
            ///
            /// @author I Copland
            ///
            /// @param The text.
            ///
            /// @return Map of chunks
            //----------------------------------------------
            std::unordered_map<std::string, std::string> GetAllChunks(const std::string& in_text)
            {
                std::unordered_map<std::string, std::string> chunks;
                u32 seekPosition = 0;
                while (seekPosition < in_text.length())
                {
                    std::string chunkName;
                    std::string chunkContents;
                    seekPosition = GetNextChunk(in_text, seekPosition, chunkName, chunkContents);
                    chunks.emplace(chunkName, chunkContents);
                }
                
                return chunks;
            }
        }
        
        CS_DEFINE_NAMEDTYPE(GLSLShaderProvider);
        
        //-------------------------------------------------------------------------
        //-------------------------------------------------------------------------
        GLSLShaderProviderUPtr GLSLShaderProvider::Create()
        {
            return GLSLShaderProviderUPtr(new GLSLShaderProvider());
        }
		//-------------------------------------------------------------------------
		//-------------------------------------------------------------------------
		bool GLSLShaderProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == GLSLShaderProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        Core::InterfaceIDType GLSLShaderProvider::GetResourceType() const
        {
            return Shader::InterfaceID;
        }
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool GLSLShaderProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
		{
			return in_extension == k_shaderFileExtension;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void GLSLShaderProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceSPtr& out_resource)
        {
            Core::FileStreamSPtr shaderStream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_read);
            if(shaderStream == nullptr || shaderStream->IsBad())
            {
                CS_LOG_ERROR("Failed to open shader file: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            std::string fileContents;
            shaderStream->GetAll(fileContents);
            shaderStream->Close();
            
            auto fileChunks = GetAllChunks(fileContents);
            
			auto chunkIt = fileChunks.find(k_languageTag);
            
            if(chunkIt == fileChunks.end())
            {
                CS_LOG_ERROR("Failed to find GLSL chunk in shader: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            auto langaugeChunks = GetAllChunks(chunkIt->second);
            
            auto vertexChunkIt = langaugeChunks.find(k_vsTag);
            if(vertexChunkIt == langaugeChunks.end())
            {
                CS_LOG_ERROR("Failed to find vertex chunk in shader: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            auto fragmentChunkIt = langaugeChunks.find(k_fsTag);
            if(fragmentChunkIt == langaugeChunks.end())
            {
                CS_LOG_ERROR("Failed to find fragment chunk in shader: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                return;
            }
            
            Shader* shader = (Shader*)(out_resource.get());
            shader->Build(vertexChunkIt->second, fragmentChunkIt->second);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        void GLSLShaderProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, Core::ResourceSPtr& out_resource)
        {
            
        }
	}
}


