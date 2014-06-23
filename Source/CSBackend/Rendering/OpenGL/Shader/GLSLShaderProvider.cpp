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

#include <CSBackend/Rendering/OpenGL/Shader/GLSLShaderProvider.h>

#include <CSBackend/Rendering/OpenGL/Shader/Shader.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

namespace CSBackend
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
            /// Grabs the contents of a chunk identified by
            /// the given tag
            ///
            ///		Tag
            ///		{
            ///			Contents
            ///		}
            ///
            /// If no chunk can be found an empty string
            /// will be returned.
            ///
            /// @author Ian Copland
            ///
            /// @param The chunk tag
            /// @param The string data to search in.
            ///
            /// @return Contents of the chunk or empty
            //----------------------------------------------
            std::string GetChunk(const std::string& in_chunkTag, const std::string& in_text)
            {
                //Find the chunk key
                u32 tagStartIdx = in_text.find(in_chunkTag);
                if(tagStartIdx == in_text.npos)
                {
                    CS_LOG_ERROR("Missing " + in_chunkTag + " tag from shader");
                    return "";
                }
                
                //Find the open brace
                u32 openBraceLocation = in_text.find("{", tagStartIdx);
                if (openBraceLocation == in_text.npos)
                {
                    CS_LOG_ERROR("Missing open brace in tag " + in_chunkTag);
                    return "";
                }
                
                //Grab all the data between the open and close braces
                u32 closeBraceLocation = openBraceLocation;
                u32 additionalOpenBraceCount = 0;
                for (u32 i = openBraceLocation + 1; i < in_text.size(); ++i)
                {
                    bool shouldContinue = true;
                    switch (in_text[i])
                    {
                        case '{':
                            ++additionalOpenBraceCount;
                            break;
                        case '}':
                            if (additionalOpenBraceCount <= 0)
                            {
                                closeBraceLocation = i;
                                shouldContinue = false;
                            }
                            else
                            {
                                --additionalOpenBraceCount;
                            }
                            break;
                        default:
                            break;
                    }
                    
                    if (shouldContinue == false)
                    {
                        break;
                    }
                }
                
                if (openBraceLocation == closeBraceLocation)
                {
                    CS_LOG_ERROR("Missing closing brace in tag " + in_chunkTag);
                    return "";
                }
                
                return in_text.substr(openBraceLocation + 1, closeBraceLocation - (openBraceLocation + 1));
            }
            //----------------------------------------------
            /// Performs the actual loading for the 2
            /// create methods
            ///
            /// @author S Downie
            ///
            /// @param Location
            /// @param File path
            /// @param Completion delegate
            /// @param [Out] Shader resource
            //----------------------------------------------
			void LoadShader(CSCore::StorageLocation in_location, const std::string& in_filePath, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const ShaderSPtr& out_shader)
            {
                CSCore::FileStreamSPtr shaderStream = CSCore::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, CSCore::FileMode::k_read);
                if(shaderStream == nullptr || shaderStream->IsBad())
                {
                    CS_LOG_ERROR("Failed to open shader file: " + in_filePath);
                    out_shader->SetLoadState(CSCore::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
						CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_shader));
                    }
                    return;
                }
                
                std::string fileContents;
                shaderStream->GetAll(fileContents);
                shaderStream->Close();
                
                std::string languageChunk = GetChunk(k_languageTag, fileContents);
                if(languageChunk.empty() == true)
                {
                    CS_LOG_ERROR("Failed to find GLSL chunk in shader: " + in_filePath);
                    out_shader->SetLoadState(CSCore::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
						CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_shader));
                    }
                    return;
                }
                
                std::string vsChunk = GetChunk(k_vsTag, languageChunk);
                if(vsChunk.empty() == true)
                {
                    CS_LOG_ERROR("Failed to find VertexShader chunk in shader: " + in_filePath);
                    out_shader->SetLoadState(CSCore::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
						CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_shader));
                    }
                    return;
                }
                
                std::string fsChunk = GetChunk(k_fsTag, languageChunk);
                if(fsChunk.empty() == true)
                {
                    CS_LOG_ERROR("Failed to find FragmentShader chunk in shader: " + in_filePath);
                    out_shader->SetLoadState(CSCore::Resource::LoadState::k_failed);
                    if(in_delegate != nullptr)
                    {
						CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_shader));
                    }
                    return;
                }
                
                if(in_delegate == nullptr)
                {
                    out_shader->Build(vsChunk, fsChunk);
                    out_shader->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                }
                else
                {
                    //All GL related tasks must be performed on the main thread.
					auto buildTask = [](const std::string& in_vs, const std::string& in_ps, const CSCore::ResourceProvider::AsyncLoadDelegate& in_completionDelegate, const ShaderSPtr& out_shaderResource)
                    {
                        out_shaderResource->Build(in_vs, in_ps);
                        out_shaderResource->SetLoadState(CSCore::Resource::LoadState::k_loaded);
                        in_completionDelegate(out_shaderResource);
                    };
					auto task = std::bind(buildTask, vsChunk, fsChunk, in_delegate, out_shader);
					CSCore::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(task);
                }
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
		bool GLSLShaderProvider::IsA(CSCore::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == GLSLShaderProvider::InterfaceID;
		}
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
        CSCore::InterfaceIDType GLSLShaderProvider::GetResourceType() const
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
		void GLSLShaderProvider::CreateResourceFromFile(CSCore::StorageLocation in_location, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceSPtr& out_resource)
        {
            ShaderSPtr shaderResource = std::static_pointer_cast<Shader>(out_resource);
            LoadShader(in_location, in_filePath, nullptr, shaderResource);
        }
        //----------------------------------------------------------------------------
        //----------------------------------------------------------------------------
		void GLSLShaderProvider::CreateResourceFromFileAsync(CSCore::StorageLocation in_location, const std::string& in_filePath, const CSCore::IResourceOptionsBaseCSPtr& in_options, const CSCore::ResourceProvider::AsyncLoadDelegate& in_delegate, const CSCore::ResourceSPtr& out_resource)
        {
            ShaderSPtr shaderResource = std::static_pointer_cast<Shader>(out_resource);
            LoadShader(in_location, in_filePath, in_delegate, shaderResource);
        }
	}
}


