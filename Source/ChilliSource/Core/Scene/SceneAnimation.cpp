//
//  SceneAnimation.cpp
//  MoshiMonsters
//
//	
//
//  Created by Andrew Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/Scene/SceneAnimation.h>
#include <ChilliSource/Core/Base/Application.h>
#include <ChilliSource/Core/Base/MakeSharedArray.h>
#include <ChilliSource/Core/Entity/Entity.h>
#include <ChilliSource/Core/Entity/EntityAnimationController.h>

namespace ChilliSource
{
    namespace Core
    {
        void SceneAnimationDesc::EntityAnimationDesc::FromBinaryStream(const Core::FileStreamSPtr & inStream)
		{
			inStream->Read((s8*)&udwKeyframeCount,sizeof(u32));
            
            afKeyframeTimes = MakeSharedArray<f32>(udwKeyframeCount);
			inStream->Read((s8*)afKeyframeTimes.get(),udwKeyframeCount * sizeof(f32));
            
            aKeyframeValues = MakeSharedArray<EntityTransform>(udwKeyframeCount);
			inStream->Read((s8*)aKeyframeValues.get(),udwKeyframeCount * sizeof(EntityTransform));
            
			u32 nNameLength;
			inStream->Read((s8*)&nNameLength,sizeof(u32));
			strName.resize(nNameLength);
			inStream->Read((s8*)&strName[0],nNameLength);
            
			u32 nPathLength;
			inStream->Read((s8*)&nPathLength,sizeof(u32));
			strTargetPath.resize(nPathLength);
			inStream->Read((s8*)&strTargetPath[0],nPathLength);
		}
		
		void SceneAnimationDesc::EntityAnimationDesc::ToBinaryStream(const Core::FileStreamSPtr & inStream)
		{
			u32 nEntries = udwKeyframeCount;
			inStream->Write((s8*)&nEntries,(s32)sizeof(u32));
            
			inStream->Write((s8*)afKeyframeTimes.get(),(s32)(nEntries * sizeof(f32)));
			inStream->Write((s8*)aKeyframeValues.get(),(s32)(nEntries * sizeof(EntityTransform)));
			
			u32 nNameLength = strName.length();
			inStream->Write((s8*)&nNameLength,sizeof(u32));
			inStream->Write((s8*)strName.data(),(s32)nNameLength);
            
			u32 nPathLength = strTargetPath.length();
			inStream->Write((s8*)&nPathLength,sizeof(u32));
			inStream->Write((s8*)strTargetPath.data(),(s32)nPathLength);
		}
        
        
        bool SceneAnimationDesc::LoadFromBinaryFile(StorageLocation ineStorageLocation, const std::string & inFilePath)
        {
            Core::FileStreamSPtr AnimFile = Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_readBinary);
            if (AnimFile->IsOpen())
            {
                u32 nAnimations;
                AnimFile->Read((s8*)&nAnimations, sizeof(u32));
                asEntityAnimations.resize(nAnimations);
                for (u32 nAnimation = 0; nAnimation < nAnimations; nAnimation++)
                    asEntityAnimations[nAnimation].FromBinaryStream(AnimFile);
                
                return true;
            }
            return false;
        }
        
        void SceneAnimationDesc::SaveToBinaryFile(StorageLocation ineStorageLocation, const std::string & inFilePath)
        {
            Core::FileStreamSPtr AnimFile= Core::Application::Get()->GetFileSystem()->CreateFileStream(ineStorageLocation, inFilePath, Core::FileMode::k_writeBinary);
			
			u32 nAnimations = asEntityAnimations.size();
			AnimFile->Write((s8*)&nAnimations, (s32)sizeof(u32));
			for (u32 nAnimation = 0; nAnimation < nAnimations; nAnimation++)
				asEntityAnimations[nAnimation].ToBinaryStream(AnimFile);
            
        }
        
        
        CS_DEFINE_NAMEDTYPE(SceneAnimation);
        
        bool SceneAnimation::IsA(InterfaceIDType inInterface) const
        {
            return  inInterface == SceneAnimation::InterfaceID;
        }
        
        bool SceneAnimation::ApplyAnimationToScene(const EntitySPtr &inpRootNode, std::vector<EntityAnimationSPtr> &outEntityAnimList) const
        {
            return ApplyAnimationToScene(inpRootNode.get(), outEntityAnimList);
        }
        
        bool SceneAnimation::ApplyAnimationToScene(Entity* inpRootNode, std::vector<EntityAnimationSPtr> &outEntityAnimList) const
        {
            if(!inpRootNode)
            {
                CS_LOG_WARNING("SceneAnimation:Tried to apply animation to nullptr Entity.");
                return false;
            }
            
            
            EntityAnimationControllerSPtr pAnimController = inpRootNode->GetEntityAnimationControllerPtr();
            
            //If we don't already have an animation controller set up, let's set one up now
            if(!pAnimController)
            {
                inpRootNode->GetEntityAnimationController() = EntityAnimationControllerSPtr(new EntityAnimationController());
                pAnimController = inpRootNode->GetEntityAnimationControllerPtr();
            }
            
            pAnimController->LoadDataFromSceneAnimDesc(msAnimation);
            
            outEntityAnimList.reserve(outEntityAnimList.size() + msAnimation.asEntityAnimations.size());
            for (u32 nAnim = 0; nAnim < msAnimation.asEntityAnimations.size(); nAnim++) 
			{				
                const SceneAnimationDesc::EntityAnimationDesc& sCurrentAnim(msAnimation.asEntityAnimations[nAnim]);                         
				Entity* pAnimEntity =  inpRootNode->FindChildEntityWithName(sCurrentAnim.strTargetPath);
				
				if (pAnimEntity)
					outEntityAnimList.push_back(pAnimController->CreateAnimation(sCurrentAnim.strName, pAnimEntity));
			}
            
            return true;
        }
        
        bool SceneAnimation::PlayAnimation(const EntitySPtr& inpRootNode, std::vector<EntityAnimationSPtr>& outEntityAnimList, AnimationPlayMode inePlayMode, InterpolationType ineInterpType) const
        {
            return PlayAnimation(inpRootNode.get(), outEntityAnimList, inePlayMode, ineInterpType);
        }
        
        bool SceneAnimation::PlayAnimation(Entity* inpRootNode, std::vector<EntityAnimationSPtr>& outEntityAnimList, AnimationPlayMode inePlayMode, InterpolationType ineInterpType) const
        {
            u32 udwCurrentAnims = outEntityAnimList.size();
            if(!ApplyAnimationToScene(inpRootNode, outEntityAnimList))
                return false;
            
            for(u32 i = udwCurrentAnims; i < outEntityAnimList.size(); i++)
            {
                outEntityAnimList[i]->SetInterpolationMode(ineInterpType);
                outEntityAnimList[i]->Play(inePlayMode);
            }
            
            return true;
        }
        
        void SceneAnimation::SetSceneAnimDesc(const SceneAnimationDesc &inAnimation)
        {
            msAnimation = inAnimation;
        }
        
    }
}
