//
//  CSAnimProvider.cpp
//  Chilli Source
//
//  Created by Ian Copland on 17/10/2011.
//  Copyright 2011 Tag Games Ltd. All rights reserved.
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
            const std::string k_fileExtension("moanim");
            
            const u32 k_minVersion = 4;
            const u32 k_maxVersion = 4;
            const u32 k_fileCheckValue = 7777;
            
            //----------------------------------------------------------------------------
            /// Read value of the given type and size from the binary file stream
            ///
			/// @author I Copland
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
            /// @author I Copland
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
                    SkinnedAnimationFrameUPtr frame(new SkinnedAnimationFrame());
                    
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
                        frame->mNodeTranslations.push_back(translation);
                        frame->mNodeOrientations.push_back(orientation);
                        frame->mNodeScalings.push_back(scale);
                    }
                    
                    //add frame to animation
                    out_resource->AddFrame(std::move(frame));
                }
            }
            //----------------------------------------------------------------------------
			/// Parses the header of the anim file.
            ///
            /// @author I Copland
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
			return in_interfaceId == ResourceProviderOld::InterfaceID || in_interfaceId == CSAnimProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSAnimProvider::CanCreateResourceOfKind(Core::InterfaceIDType in_interfaceId) const
		{
			return (in_interfaceId == SkinnedAnimation::InterfaceID);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSAnimProvider::CanCreateResourceFromFileWithExtension(const std::string& in_extension) const
		{
			return (in_extension == k_fileExtension);
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSAnimProvider::CreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
		{
			SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
			anim->SetLoaded(false);
            
            ReadSkinnedAnimationFromFile(in_location, in_filePath, anim);
            return anim->IsLoaded();
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		bool CSAnimProvider::AsyncCreateResourceFromFile(Core::StorageLocation in_location, const std::string& in_filePath, Core::ResourceOldSPtr& out_resource)
		{
			SkinnedAnimationSPtr anim = std::static_pointer_cast<SkinnedAnimation>(out_resource);
			anim->SetLoaded(false);
            
			//Load model as task
			Core::Task<Core::StorageLocation, const std::string&, const SkinnedAnimationSPtr&> AnimTask(this, &CSAnimProvider::ReadSkinnedAnimationFromFile,in_location, in_filePath, anim);
			Core::TaskScheduler::ScheduleTask(AnimTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		//----------------------------------------------------------------------------
		void CSAnimProvider::ReadSkinnedAnimationFromFile(Core::StorageLocation in_location, const std::string& in_filePath, const SkinnedAnimationSPtr& out_resource) const
		{
			Core::FileStreamSPtr stream = Core::Application::Get()->GetFileSystem()->CreateFileStream(in_location, in_filePath, Core::FileMode::k_readBinary);
			
			u32 numFrames = 0;
			s32 numSkeletonNodes = 0;
			if(ReadHeader(stream, in_filePath, out_resource, numFrames, numSkeletonNodes) == false)
            {
                return;
            }
            
			ReadAnimationData(stream, numFrames, numSkeletonNodes, out_resource);
			
			out_resource->SetLoaded(true);
		}
	}
}
