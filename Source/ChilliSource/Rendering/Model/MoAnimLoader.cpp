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
#include <ChilliSource/Core/Main/Application.h>
#include <ChilliSource/Core/Threading/TaskScheduler.h>

#include <stdexcept>

namespace moFlo
{
	namespace Rendering
	{
		const std::string kstrMoAnimExtension("moanim");
		
		const u32 kgMoAnimMinVersion = 4;
		const u32 kgMoAnimMaxVersion = 4;
		const u32 kgMoAnimFileCheckValue = 7777;
		
		enum MO_ANIM_FEATURE
		{
			MAF_NONE
		};
		
		//-------------------------------------------------------------------------
		/// Constructor
		//-------------------------------------------------------------------------
		CMoAnimLoader::CMoAnimLoader(Core::CApplication* inpApp) : mpApp(inpApp)
		{
		}
		//-------------------------------------------------------------------------
		/// Is A
		//-------------------------------------------------------------------------
		bool CMoAnimLoader::IsA(Core::InterfaceIDType inInterfaceID) const
		{
			return inInterfaceID == IResourceProvider::InterfaceID;
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource of Kind
		//----------------------------------------------------------------------------
		bool CMoAnimLoader::CanCreateResourceOfKind(Core::InterfaceIDType inInterfaceID) const
		{
			return (inInterfaceID == Rendering::CSkinnedAnimation::InterfaceID);
		}
		//----------------------------------------------------------------------------
		/// Can Create Resource From File With Extension
		//----------------------------------------------------------------------------
		bool CMoAnimLoader::CanCreateResourceFromFileWithExtension(const std::string & inExtension) const
		{
			return (inExtension == kstrMoAnimExtension);
		}
		//----------------------------------------------------------------------------
		/// Create Resource From File
		//----------------------------------------------------------------------------
		bool CMoAnimLoader::CreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)  
		{
			SkinnedAnimationPtr pAnim = SHARED_PTR_CAST<CSkinnedAnimation>(outpResource);
			
			return CreateSkinnedAnimationFromFile(ineStorageLocation, inFilePath, pAnim);
		}
		//----------------------------------------------------------------------------
		/// Async Create Resource From File
		//----------------------------------------------------------------------------
		bool CMoAnimLoader::AsyncCreateResourceFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, Core::ResourcePtr& outpResource)
		{
			SkinnedAnimationPtr pAnim = SHARED_PTR_CAST<CSkinnedAnimation>(outpResource);
			
			//Load model as task
			Task3<Core::STORAGE_LOCATION, const std::string&, const SkinnedAnimationPtr&> AnimTask(this, &CMoAnimLoader::ReadAnimationTask,ineStorageLocation, inFilePath, pAnim);
			CTaskScheduler::ScheduleTask(AnimTask);
			
			return true;
		}
		//----------------------------------------------------------------------------
		/// ReadAnimationTask
		//----------------------------------------------------------------------------
		void CMoAnimLoader::ReadAnimationTask(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationPtr& outpResource)
		{
			CreateSkinnedAnimationFromFile(ineStorageLocation, inFilePath, outpResource);
		}
		//----------------------------------------------------------------------------
		/// Create Skinned Animation From File
		//----------------------------------------------------------------------------
		bool CMoAnimLoader::CreateSkinnedAnimationFromFile(Core::STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath, const SkinnedAnimationPtr& outpResource)  
		{
            bool mbSuccess = true;
			
			moFlo::Core::FileStreamPtr stream = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, moFlo::Core::FM_READ_BINARY);
			
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
		bool CMoAnimLoader::ReadHeader(const moFlo::Core::FileStreamPtr& inpStream, const std::string & inFilePath, const SkinnedAnimationPtr& outpResource, u32& outudwNumFrames, s32& outdwNumSkeletonNodes)
		{
			//Check file for corruption
			if(inpStream == NULL || inpStream->IsBad() == true)
			{
				ERROR_LOG("Cannot open MoAnim file: " + inFilePath);
				return false;
			}
			
			u32 udwFileCheckValue = ReadValue<u32>(inpStream);
			if(udwFileCheckValue != kgMoAnimFileCheckValue)
			{
				ERROR_LOG("MoAnim file has corruption(incorrect File Check Value): " + inFilePath);
				return false;
			}
			
			u32 udwVersionNum = ReadValue<u32>(inpStream);
			if (udwVersionNum < kgMoAnimMinVersion || udwVersionNum > kgMoAnimMaxVersion)
			{
				ERROR_LOG("Unsupported MoAnim version: " + inFilePath);
				return false;
			}
			
			//build the feature declaration from the file
			u32 dwNumFeatures = (u32)ReadValue<u8>(inpStream);
			for (u32 i = 0; i < dwNumFeatures; i++)
			{
				u32 dwFeatureType = (u32)ReadValue<u8>(inpStream);
				
				switch (dwFeatureType)
				{
					default:
						ERROR_LOG("Unknown feature type in MoAnim(" + inFilePath + ")feature declaration!");
						break;
				}
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
		bool CMoAnimLoader::ReadAnimationData(const moFlo::Core::FileStreamPtr& inpStream, u32 inudwNumFrames, s32 indwNumSkeletonNodes, const SkinnedAnimationPtr& outpResource)
		{
			for (u32 i = 0; i < inudwNumFrames; i++)
			{
				//create new frame
				SkinnedAnimationFramePtr frame(new SkinnedAnimationFrame());
				
				//add all skeleton nodes matrices
				for (s32 j = 0; j < indwNumSkeletonNodes; j++)
				{
					//create new translation
					Core::CVector3 translation;
					translation.x = ReadValue<f32>(inpStream);
					translation.y = ReadValue<f32>(inpStream);
					translation.z = ReadValue<f32>(inpStream);
	
					//create new orientation
					Core::CQuaternion orientation;
					orientation.x = ReadValue<f32>(inpStream);
					orientation.y = ReadValue<f32>(inpStream);
					orientation.z = ReadValue<f32>(inpStream);
					orientation.w = ReadValue<f32>(inpStream);
                    
                    //create new scale
					Core::CVector3 scale;
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
		CMoAnimLoader::~CMoAnimLoader()
		{
		}
	}
}
