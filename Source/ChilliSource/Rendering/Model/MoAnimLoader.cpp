/*
 *  MoAnimLoader.cpp
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#include <ChilliSource/Rendering/Model/MoAnimLoader.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimation.h>
#include <ChilliSource/Core/Math/Vector3.h>
#include <ChilliSource/Core/Math/Quaternion.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <stdexcept>

namespace ChilliSource
{
	namespace Rendering
	{
		const std::string kstrMoAnimExtension("moanim");
		
		const u32 kgMoAnimMinVersion = 4;
		const u32 kgMoAnimMaxVersion = 4;
		const u32 kgMoAnimFileCheckValue = 7777;
		
		enum class MoAnimFeature
		{
			k_none
		};
		
        MoAnimLoaderUPtr MoAnimLoader::Create()
        {
            return MoAnimLoaderUPtr(new MoAnimLoader());
        }
        
		//-------------------------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------------------------
		MoAnimLoader::MoAnimLoader()
		{
		}
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool MoAnimLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == ResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		//----------------------------------------------------------------------------
		bool MoAnimLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == SkinnedAnimation::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------------------
		bool MoAnimLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoAnimExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------------------
		bool MoAnimLoader::CreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)  
		{
			SkinnedAnimationSPtr pAnim = std::static_pointer_cast<SkinnedAnimation>(outpResource);
			
			return CreateSkinnedAnimationFromFile(ineStorageLocation, inFilePath, pAnim);
		}
		//----------------------------------------------------------------------------
		/// Async Create Resource From File
		//----------------------------------------------------------------------------
		bool MoAnimLoader::AsyncCreateResourceFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, Core::ResourceSPtr& outpResource)
		{
			SkinnedAnimationSPtr pAnim = std::static_pointer_cast<SkinnedAnimation>(outpResource);
			
			//Load model as task
			Core::Task<Core::StorageLocation, const std::string&, const SkinnedAnimationSPtr&> AnimTask(this, &MoAnimLoader::ReadAnimationTask,ineStorageLocation, inFilePath, pAnim);
			Core::TaskScheduler::ScheduleTask(AnimTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		/// ReadAnimationTask
		//----------------------------------------------------------------------------
		void MoAnimLoader::ReadAnimationTask(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource)
		{
			CreateSkinnedAnimationFromFile(ineStorageLocation, inFilePath, outpResource);
		}
		//----------------------------------------------------------------------------
		/// Create Skinned Animation From File
		//----------------------------------------------------------------------------
		bool MoAnimLoader::CreateSkinnedAnimationFromFile(Core::StorageLocation ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource)  
		{
            bool mbSuccess = true;
			
			Core::FileStreamSPtr stream = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
			
			u32 udwNumFrames = 0;
			s32 dwNumSkeletonNodes = 0;
			mbSuccess = ReadHeader(stream,inFilePath, outpResource, udwNumFrames, dwNumSkeletonNodes);
			
			if (mbSuccess == true)
				mbSuccess = ReadAnimationData(stream, udwNumFrames, dwNumSkeletonNodes, outpResource);	
			
			//cleanup
			stream->Close();
			outpResource->SetLoaded(true);
			
			//return true
			return mbSuccess;
		}
		//----------------------------------------------------------------------------
		/// Read Header
		//----------------------------------------------------------------------------
		bool MoAnimLoader::ReadHeader(const ChilliSource::Core::FileStreamSPtr& inpStream, const std::string & inFilePath, const SkinnedAnimationSPtr& outpResource, u32& outudwNumFrames, s32& outdwNumSkeletonNodes)
		{
			//Check file for corruption
			if(inpStream == nullptr || inpStream->IsBad() == true)
			{
				CS_LOG_ERROR("Cannot open MoAnim file: " + inFilePath);
				return false;
			}
			
			u32 udwFileCheckValue = ReadValue<u32>(inpStream);
			if(udwFileCheckValue != kgMoAnimFileCheckValue)
			{
				CS_LOG_ERROR("MoAnim file has corruption(incorrect File Check Value): " + inFilePath);
				return false;
			}
			
			u32 udwVersionNum = ReadValue<u32>(inpStream);
			if (udwVersionNum < kgMoAnimMinVersion || udwVersionNum > kgMoAnimMaxVersion)
			{
				CS_LOG_ERROR("Unsupported MoAnim version: " + inFilePath);
				return false;
			}
			
			//build the feature declaration from the file
			u32 dwNumFeatures = (u32)ReadValue<u8>(inpStream);
			if (dwNumFeatures != 0)
			{
				CS_LOG_ERROR("Unknown feature type in MoAnim(" + inFilePath + ")feature declaration!");
			}
			
			//read num frames and skeleton nodes
			outudwNumFrames = (u32)ReadValue<u16>(inpStream);
			outdwNumSkeletonNodes = (s32)ReadValue<s16>(inpStream);
			
			//read frame time
			f32 fFrameTime = ReadValue<f32>(inpStream);
			outpResource->SetFrameTime(fFrameTime);
			return true;
		}
		//----------------------------------------------------------------------------
		/// Read Animation Data
		//----------------------------------------------------------------------------
		bool MoAnimLoader::ReadAnimationData(const ChilliSource::Core::FileStreamSPtr& inpStream, u32 inudwNumFrames, s32 indwNumSkeletonNodes, const SkinnedAnimationSPtr& outpResource)
		{
			for (u32 i = 0; i < inudwNumFrames; i++)
			{
				//create new frame
				SkinnedAnimationFrameSPtr frame(new SkinnedAnimationFrame());
				
				//add all skeleton nodes matrices
				for (s32 j = 0; j < indwNumSkeletonNodes; j++)
				{
					//create new translation
					Core::Vector3 translation;
					translation.x = ReadValue<f32>(inpStream);
					translation.y = ReadValue<f32>(inpStream);
					translation.z = ReadValue<f32>(inpStream);
	
					//create new orientation
					Core::Quaternion orientation;
					orientation.x = ReadValue<f32>(inpStream);
					orientation.y = ReadValue<f32>(inpStream);
					orientation.z = ReadValue<f32>(inpStream);
					orientation.w = ReadValue<f32>(inpStream);
                    
                    //create new scale
					Core::Vector3 scale;
					scale.x = ReadValue<f32>(inpStream);
					scale.y = ReadValue<f32>(inpStream);
					scale.z = ReadValue<f32>(inpStream);
					
					//add to the frame
					frame->mNodeTranslations.push_back(translation);
					frame->mNodeOrientations.push_back(orientation);
                    frame->mNodeScalings.push_back(scale);
				}
				
				//add frame to animation
				outpResource->AddFrame(frame);
			}
			return true;
		}
		//-------------------------------------------------------------------------
		/// Destructor
		//-------------------------------------------------------------------------
		MoAnimLoader::~MoAnimLoader()
		{
		}
	}
}
