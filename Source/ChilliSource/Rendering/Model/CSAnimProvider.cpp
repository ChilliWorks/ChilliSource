//
//  CSAnimProvider.cpp
//  Chilli Source
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
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>

namespace ChilliSource
{
	namespace Rendering
	{
        namespace
        {
            const std::string k_fileExtension("csanim");
            
            const u32 k_minVersion = 4;
            const u32 k_maxVersion = 4;
            const u32 k_fileCheckValue = 7777;
            
            //----------------------------------------------------------------------------
            /// Read value of the given type and size from the binary file stream
            ///
			/// @author Ian Copland
			///
			/// @param File stream
			/// @return Value of type TType
			//----------------------------------------------------------------------------
			template <typename TType> TType ReadValue(const Core::FileStreamSPtr& in_fileStream)
			{
				TType value;
				in_fileStream->Read(reinterpret_cast<s8*>(&value), sizeof(TType));
				return value;
			}
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
            void ReadAnimationData(const Core::FileStreamSPtr& in_fileStream, u32 in_numFrames, s32 in_numSkeletonNodes, const SkinnedAnimationSPtr& out_resource)
            {
                for (u32 frameCount=0; frameCount<in_numFrames; ++frameCount)
                {
                    //create new frame
                    SkinnedAnimation::FrameUPtr frame(new SkinnedAnimation::Frame());
                    
                    //add all skeleton nodes matrices
                    for (u32 skelNodeCount=0; skelNodeCount<(u32)in_numSkeletonNodes; ++skelNodeCount)
                    {
                        //create new translation
                        Core::Vector3 translation;
                        translation.x = ReadValue<f32>(in_fileStream);
                        translation.y = ReadValue<f32>(in_fileStream);
                        translation.z = ReadValue<f32>(in_fileStream);
                        
                        //create new orientation
                        Core::Quaternion orientation;
                        orientation.x = ReadValue<f32>(in_fileStream);
                        orientation.y = ReadValue<f32>(in_fileStream);
                        orientation.z = ReadValue<f32>(in_fileStream);
                        orientation.w = ReadValue<f32>(in_fileStream);
                        
                        //create new scale
                        Core::Vector3 scale;
                        scale.x = ReadValue<f32>(in_fileStream);
                        scale.y = ReadValue<f32>(in_fileStream);
                        scale.z = ReadValue<f32>(in_fileStream);
                        
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
			bool ReadHeader(const Core::FileStreamSPtr& in_stream, const std::string & in_filePath, const SkinnedAnimationSPtr& out_resource, u32& out_numFrames, s32& out_numSkeletonNodes)
            {
                //Check file for corruption
                if(in_stream == nullptr || in_stream->IsBad() == true)
                {
                    CS_LOG_ERROR("Cannot open CSAnim file: " + in_filePath);
                    return false;
                }
                
                u32 fileCheckValue = ReadValue<u32>(in_stream);
                if(fileCheckValue != k_fileCheckValue)
                {
                    CS_LOG_ERROR("CSAnim file has corruption(incorrect File Check Value): " + in_filePath);
                    return false;
                }
                
                u32 versionNum = ReadValue<u32>(in_stream);
                if (versionNum < k_minVersion || versionNum > k_maxVersion)
                {
                    CS_LOG_ERROR("Unsupported CSAnim version: " + in_filePath);
                    return false;
                }
                
                //build the feature declaration from the file
                u32 numFeatures = (u32)ReadValue<u8>(in_stream);
                if (numFeatures != 0)
                {
                    CS_LOG_ERROR("Unknown feature type in CSAnim (" + in_filePath + ") feature declaration!");
                }
                
                //read num frames and skeleton nodes
                out_numFrames = (u32)ReadValue<u16>(in_stream);
                out_numSkeletonNodes = (s32)ReadValue<s16>(in_stream);
                
                //read frame time
                f32 frameTime = ReadValue<f32>(in_stream);
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
		bool CSAnimProvider::IsA(Core::InterfaceIDType in_interfaceId) const
		{
			return in_interfaceId == ResourceProvider::InterfaceID || in_interfaceId == CSAnimProvider::InterfaceID;
		}
        //----------------------------------------------------
        //----------------------------------------------------
        Core::InterfaceIDType CSAnimProvider::GetResourceType() const
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
		void CSAnimProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceSPtr& out_resource)
		{
			SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
            
            ReadSkinnedAnimationFromFile(in_location, in_filePath, nullptr, anim);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSAnimProvider::CreateResourceFromFileAsync(Core::StorageLocation in_location, const std::string& in_filePath, const Core::IResourceOptionsBaseCSPtr& in_options, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const Core::ResourceSPtr& out_resource)
		{
			SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
			auto task = std::bind(&CSAnimProvider::ReadSkinnedAnimationFromFile, this, in_location, in_filePath, in_delegate, anim);
			Core::Application::Get()->GetTaskScheduler()->ScheduleTask(task);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSAnimProvider::ReadSkinnedAnimationFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const Core::ResourceProvider::AsyncLoadDelegate& in_delegate, const SkinnedAnimationSPtr& out_resource) const
		{
			Core::FileStreamSPtr stream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_readBinary);
			
			u32 numFrames = 0;
			s32 numSkeletonNodes = 0;
			if(ReadHeader(stream, in_filePath, out_resource, numFrames, numSkeletonNodes) == false)
            {
                CS_LOG_ERROR("Failed to read header in anim: " + in_filePath);
                out_resource->SetLoadState(Core::Resource::LoadState::k_failed);
                if(in_delegate != nullptr)
                {
					Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
                }
                return;
            }
            
			ReadAnimationData(stream, numFrames, numSkeletonNodes, out_resource);
			
            out_resource->SetLoadState(Core::Resource::LoadState::k_loaded);
            
            if(in_delegate != nullptr)
            {
				Core::Application::Get()->GetTaskScheduler()->ScheduleMainThreadTask(std::bind(in_delegate, out_resource));
            }
		}
	}
}
