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

#include <ChilliSource/Rendering/Shader/CSShaderProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Shader/Shader.h>

namespace ChilliSource
{
    namespace
    {
        const std::string k_shaderFileExtension("csshader");
        const std::string k_languageTag("GLSL");
        const std::string k_vsTag("VertexShader");
        const std::string k_fsTag("FragmentShader");
        
        /// Grabs the contents of a chunk identified by the given tag
        ///
        ///		Tag
        ///		{
        ///			Contents
        ///		}
        ///
        /// If no chunk can be found an empty string will be returned.
        ///
        /// @param chunkTag
        ///     The chunk tag
        /// @param text
        ///     The string data to search in.
        ///
        /// @return Contents of the chunk or empty
        ///
        std::string GetChunk(const std::string& chunkTag, const std::string& text) noexcept
        {
            //Find the chunk key
            auto tagStartIdx = text.find(chunkTag);
            CS_ASSERT(tagStartIdx != text.npos, "Missing '" + chunkTag + "' tag from shader.");
            
            //Find the open brace
            auto openBraceLocation = text.find("{", tagStartIdx);
            CS_ASSERT(openBraceLocation != text.npos, "Missing open brace in tag '" + chunkTag + "'.");
            
            //Grab all the data between the open and close braces
            auto closeBraceLocation = openBraceLocation;
            u32 additionalOpenBraceCount = 0;
            for (auto i = openBraceLocation + 1; i < text.size(); ++i)
            {
                bool shouldContinue = true;
                switch (text[i])
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
            
            CS_ASSERT(openBraceLocation != closeBraceLocation, "Missing closing brace in tag '" + chunkTag + "'.");
            return text.substr(openBraceLocation + 1, closeBraceLocation - (openBraceLocation + 1));
        }
        
        /// Performs the actual loading for the 2 create methods
        ///
        /// @param location
        ///     The storage location.
        /// @param filePath
        ///     The file path.
        /// @param delegate
        ///     The Completion delegate
        /// @param shader
        ///     The output shader resource.
        ///
        void LoadShader(StorageLocation location, const std::string& filePath, const ResourceProvider::AsyncLoadDelegate& delegate, const ShaderSPtr& shader) noexcept
        {
            auto shaderStream = Application::Get()->GetFileSystem()->CreateTextInputStream(location, filePath);
            if(shaderStream == nullptr)
            {
                CS_LOG_ERROR("Failed to open shader file: " + filePath);
                shader->SetLoadState(Resource::LoadState::k_failed);
                if(delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                    {
                        delegate(shader);
                    });
                }
                return;
            }
            
            std::string fileContents = shaderStream->ReadAll();
            shaderStream.reset();
            
            std::string languageChunk = GetChunk(k_languageTag, fileContents);
            if (languageChunk.empty() == true)
            {
                CS_LOG_ERROR("Failed to find GLSL chunk in shader: " + filePath);
                shader->SetLoadState(Resource::LoadState::k_failed);
                if (delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                    {
                        delegate(shader);
                    });
                }
                return;
            }
            
            std::string vsChunk = GetChunk(k_vsTag, languageChunk);
            if (vsChunk.empty() == true)
            {
                CS_LOG_ERROR("Failed to find VertexShader chunk in shader: " + filePath);
                shader->SetLoadState(Resource::LoadState::k_failed);
                if (delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                    {
                        delegate(shader);
                    });
                }
                return;
            }
            
            std::string fsChunk = GetChunk(k_fsTag, languageChunk);
            if (fsChunk.empty() == true)
            {
                CS_LOG_ERROR("Failed to find FragmentShader chunk in shader: " + filePath);
                shader->SetLoadState(Resource::LoadState::k_failed);
                if (delegate != nullptr)
                {
                    Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                    {
                        delegate(shader);
                    });
                }
                return;
            }
            
            if (delegate == nullptr)
            {
                shader->Build(vsChunk, fsChunk);
                shader->SetLoadState(Resource::LoadState::k_loaded);
            }
            else
            {
                //All GL related tasks must be performed on the main thread.
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                {
                    shader->Build(vsChunk, fsChunk);
                    shader->SetLoadState(Resource::LoadState::k_loaded);
                    delegate(shader);
                });
            }
        }
    }
    
    CS_DEFINE_NAMEDTYPE(CSShaderProvider);
    
    //------------------------------------------------------------------------------
    CSShaderProviderUPtr CSShaderProvider::Create() noexcept
    {
        return CSShaderProviderUPtr(new CSShaderProvider());
    }
    
    //------------------------------------------------------------------------------
    bool CSShaderProvider::IsA(InterfaceIDType interfaceId) const noexcept
    {
        return interfaceId == ResourceProvider::InterfaceID || interfaceId == CSShaderProvider::InterfaceID;
    }
    
    //------------------------------------------------------------------------------
    InterfaceIDType CSShaderProvider::GetResourceType() const noexcept
    {
        return Shader::InterfaceID;
    }
    
    //------------------------------------------------------------------------------
    bool CSShaderProvider::CanCreateResourceWithFileExtension(const std::string& extension) const noexcept
    {
        return extension == k_shaderFileExtension;
    }
    
    //------------------------------------------------------------------------------
    void CSShaderProvider::CreateResourceFromFile(StorageLocation location, const std::string& filePath, const IResourceOptionsBaseCSPtr& options, const ResourceSPtr& resource) noexcept
    {
        ShaderSPtr shaderResource = std::static_pointer_cast<Shader>(resource);
        LoadShader(location, filePath, nullptr, shaderResource);
    }
    
    //------------------------------------------------------------------------------
    void CSShaderProvider::CreateResourceFromFileAsync(StorageLocation location, const std::string& filePath, const IResourceOptionsBaseCSPtr& options, const ResourceProvider::AsyncLoadDelegate& delegate, const ResourceSPtr& resource) noexcept
    {
        ShaderSPtr shaderResource = std::static_pointer_cast<Shader>(resource);
        LoadShader(location, filePath, delegate, shaderResource);
    }
}


