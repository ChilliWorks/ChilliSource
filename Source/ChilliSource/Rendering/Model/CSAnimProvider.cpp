//
//  CSAnimProvider.cpp
//  ChilliSource
//  Created by Ian Copland on 17/10/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
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

#include <ChilliSource/Rendering/Model/CSAnimProvider.h>

#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/File.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace ChilliSource
{
    namespace
    {
        const std::string k_fileExtension("csanim");
        
        const u32 k_minVersion = 4;
        const u32 k_maxVersion = 4;
        const u32 k_fileCheckValue = 7777;
        
        //----------------------------------------------------------------------------
        /// Reads all of the data for the animation into the SkinnedAnimation resource
        ///
        /// @author Ian Copland
        ///
        /// @param The file stream.
        /// @param The number of frames.
        /// @param The number of skeleton nodes.
        /// @param [Out] Animation resource to populate
        //----------------------------------------------------------------------------
        void ReadAnimationData(const IBinaryInputStreamUPtr& in_fileStream, u32 in_numFrames, s32 in_numSkeletonNodes, const SkinnedAnimationSPtr& out_resource)
        {
            for (u32 frameCount=0; frameCount<in_numFrames; ++frameCount)
            {
                //create new frame
                SkinnedAnimation::FrameUPtr frame(new SkinnedAnimation::Frame());
                
                //add all skeleton nodes matrices
                for (u32 skelNodeCount=0; skelNodeCount<(u32)in_numSkeletonNodes; ++skelNodeCount)
                {
                    //create new translation
                    Vector3 translation;
                    translation.x = in_fileStream->Read<f32>();
                    translation.y = in_fileStream->Read<f32>();
                    translation.z = in_fileStream->Read<f32>();
                    
                    //create new orientation
                    Quaternion orientation;
                    orientation.x = in_fileStream->Read<f32>();
                    orientation.y = in_fileStream->Read<f32>();
                    orientation.z = in_fileStream->Read<f32>();
                    orientation.w = in_fileStream->Read<f32>();
                    
                    //create new scale
                    Vector3 scale;
                    scale.x = in_fileStream->Read<f32>();
                    scale.y = in_fileStream->Read<f32>();
                    scale.z = in_fileStream->Read<f32>();
                    
                    //add to the frame
                    frame->m_nodeTranslations.push_back(translation);
                    frame->m_nodeOrientations.push_back(orientation);
                    frame->m_nodeScales.push_back(scale);
                }
                
                //add frame to animation
                out_resource->AddFrame(std::move(frame));
            }
        }
        //----------------------------------------------------------------------------
        /// Parses the header of the anim file.
        ///
        /// @author Ian Copland
        ///
        /// @param The file stream.
        /// @param the Skeletal Animation that this data is being loaded into.
        ///
        /// @return whether or not this was successful
        //----------------------------------------------------------------------------
        bool ReadHeader(const IBinaryInputStreamUPtr& in_stream, const std::string & in_filePath, const SkinnedAnimationSPtr& out_resource, u32& out_numFrames, s32& out_numSkeletonNodes)
        {
            //Check file for corruption
            if(in_stream == nullptr)
            {
                CS_LOG_ERROR("Cannot open CSAnim file: " + in_filePath);
                return false;
            }
            
            u32 fileCheckValue = in_stream->Read<u32>();
            if(fileCheckValue != k_fileCheckValue)
            {
                CS_LOG_ERROR("CSAnim file has corruption(incorrect File Check Value): " + in_filePath);
                return false;
            }
            
            u32 versionNum = in_stream->Read<u32>();
            if (versionNum < k_minVersion || versionNum > k_maxVersion)
            {
                CS_LOG_ERROR("Unsupported CSAnim version: " + in_filePath);
                return false;
            }
            
            //build the feature declaration from the file
            u32 numFeatures = (u32)in_stream->Read<u8>();
            if (numFeatures != 0)
            {
                CS_LOG_ERROR("Unknown feature type in CSAnim (" + in_filePath + ") feature declaration!");
            }
            
            //read num frames and skeleton nodes
            out_numFrames = (u32)in_stream->Read<u16>();
            out_numSkeletonNodes = (s32)in_stream->Read<s16>();
            
            //read frame time
            f32 frameTime = in_stream->Read<f32>();
            out_resource->SetFrameTime(frameTime);
            return true;
        }
    }
    
    CS_DEFINE_NAMEDTYPE(CSAnimProvider);
    
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    CSAnimProviderUPtr CSAnimProvider::Create()
    {
        return CSAnimProviderUPtr(new CSAnimProvider());
    }
    //-------------------------------------------------------------------------
    //-------------------------------------------------------------------------
    bool CSAnimProvider::IsA(InterfaceIDType in_interfaceId) const
    {
        return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSAnimProvider::InterfaceID;
    }
    //----------------------------------------------------
    //----------------------------------------------------
    InterfaceIDType CSAnimProvider::GetResourceType() const
    {
        return SkinnedAnimation::InterfaceID;
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    bool CSAnimProvider::CanCreateResourceWithFileExtension(const std::string& in_extension) const
    {
        return (in_extension == k_fileExtension);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSAnimProvider::CreateResourceFromFile(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceSPtr& out_resource)
    {
        SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
        
        ReadSkinnedAnimationFromFile(in_location, in_filePath, nullptr, anim);
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSAnimProvider::CreateResourceFromFileAsync(StorageLocation in_location, const std::string& in_filePath, const IResourceOptionsBaseCSPtr& in_options, const ResourceProvider::AsyncLoadDelegate& in_delegate, const ResourceSPtr& out_resource)
    {
        SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
        Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_file, [=](const TaskContext&) noexcept
        {
            ReadSkinnedAnimationFromFile(in_location, in_filePath, in_delegate, anim);
        });
    }
    //----------------------------------------------------------------------------
    //----------------------------------------------------------------------------
    void CSAnimProvider::ReadSkinnedAnimationFromFile(StorageLocation in_location, const std::string& in_filePath, const ResourceProvider::AsyncLoadDelegate& in_delegate, const SkinnedAnimationSPtr& out_resource) const
    {
        IBinaryInputStreamUPtr stream = Application::Get()->GetFileSystem()->CreateBinaryInputStream(in_location, in_filePath);

        u32 numFrames = 0;
        s32 numSkeletonNodes = 0;
        if(ReadHeader(stream, in_filePath, out_resource, numFrames, numSkeletonNodes) == false)
        {
            CS_LOG_ERROR("Failed to read header in anim: " + in_filePath);
            out_resource->SetLoadState(Resource::LoadState::k_failed);
            if(in_delegate != nullptr)
            {
                Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
                {
                    in_delegate(out_resource);
                });
            }
            return;
        }
        
        ReadAnimationData(stream, numFrames, numSkeletonNodes, out_resource);
        
        out_resource->SetLoadState(Resource::LoadState::k_loaded);
        
        if(in_delegate != nullptr)
        {
            Application::Get()->GetTaskScheduler()->ScheduleTask(TaskType::k_mainThread, [=](const TaskContext&) noexcept
            {
                in_delegate(out_resource);
            });
        }
    }
}
