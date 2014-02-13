//
//  SceneAnimation.cpp
//  MoshiMonsters
//
//	
//
//  Created by Andrew Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#include <ChilliSource/Core/SceneAnimation/SceneAnimation.h>
#include <ChilliSource/Core/Application.h>
#include <ChilliSource/Core/Entity.h>
#include <ChilliSource/Core/EntityAnimationController.h>

namespace moFlo
{
    namespace Core
    {
        void CSceneAnimationDesc::EntityAnimationDesc::FromBinaryStream(const Core::FileStreamPtr & inStream)
		{
			inStream->Read((s8*)&udwKeyframeCount,sizeof(u32));
            
			afKeyframeTimes = SHARED_ARRAY_PTR<f32>(new f32[udwKeyframeCount]);
			inStream->Read((s8*)&afKeyframeTimes[0],udwKeyframeCount * sizeof(f32));
            
            aKeyframeValues = SHARED_ARRAY_PTR<EntityTransform>(new EntityTransform[udwKeyframeCount]);
			inStream->Read((s8*)&aKeyframeValues[0],udwKeyframeCount * sizeof(EntityTransform));
            
			u32 nNameLength;
			inStream->Read((s8*)&nNameLength,sizeof(u32));
			strName.resize(nNameLength);
			inStream->Read((s8*)&strName[0],nNameLength);
            
			u32 nPathLength;
			inStream->Read((s8*)&nPathLength,sizeof(u32));
			strTargetPath.resize(nPathLength);
			inStream->Read((s8*)&strTargetPath[0],nPathLength);
		}
		
		void CSceneAnimationDesc::EntityAnimationDesc::ToBinaryStream(const Core::FileStreamPtr & inStream)
		{
			u32 nEntries = udwKeyframeCount;
			inStream->Write((s8*)&nEntries,(s32)sizeof(u32));
            
			inStream->Write((s8*)&afKeyframeTimes[0],(s32)(nEntries * sizeof(f32)));
			inStream->Write((s8*)&aKeyframeValues[0],(s32)(nEntries * sizeof(EntityTransform)));
			
			u32 nNameLength = strName.length();
			inStream->Write((s8*)&nNameLength,sizeof(u32));
			inStream->Write((s8*)strName.data(),(s32)nNameLength);
            
			u32 nPathLength = strTargetPath.length();
			inStream->Write((s8*)&nPathLength,sizeof(u32));
			inStream->Write((s8*)strTargetPath.data(),(s32)nPathLength);
		}
        
        
        bool CSceneAnimationDesc::LoadFromBinaryFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
        {
            Core::FileStreamPtr AnimFile = Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FM_READ_BINARY);
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
        
        void CSceneAnimationDesc::SaveToBinaryFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath)
        {
            Core::FileStreamPtr AnimFile= Core::CApplication::GetFileSystemPtr()->CreateFileStream(ineStorageLocation, inFilePath, Core::FM_WRITE_BINARY);
			
			u32 nAnimations = asEntityAnimations.size();
			AnimFile->Write((s8*)&nAnimations, (s32)sizeof(u32));
			for (u32 nAnimation = 0; nAnimation < nAnimations; nAnimation++)
				asEntityAnimations[nAnimation].ToBinaryStream(AnimFile);
            
        }
        
        
        DEFINE_NAMED_INTERFACE(CSceneAnimation);
        
        bool CSceneAnimation::IsA(InterfaceIDType inInterface) const
        {
            return  inInterface == CSceneAnimation::InterfaceID;
        }
        
        bool CSceneAnimation::ApplyAnimationToScene(const EntityPtr &inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr> &outEntityAnimList) const
        {
            return ApplyAnimationToScene(inpRootNode.get(), outEntityAnimList);
        }
        
        bool CSceneAnimation::ApplyAnimationToScene(CEntity* inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr> &outEntityAnimList) const
        {
            if(!inpRootNode)
            {
                WARNING_LOG("CSceneAnimation:Tried to apply animation to NULL Entity.");
                return false;
            }
            
            
            EntityAnimationControllerPtr pAnimController = inpRootNode->GetEntityAnimationControllerPtr();
            
            //If we don't already have an animation controller set up, let's set one up now
            if(!pAnimController)
            {
                inpRootNode->GetEntityAnimationController() = EntityAnimationControllerPtr(new CEntityAnimationController());
                pAnimController = inpRootNode->GetEntityAnimationControllerPtr();
            }
            
            pAnimController->LoadDataFromSceneAnimDesc(msAnimation);
            
            outEntityAnimList.reserve(outEntityAnimList.size() + msAnimation.asEntityAnimations.size());
            for (u32 nAnim = 0; nAnim < msAnimation.asEntityAnimations.size(); nAnim++) 
			{				
                const CSceneAnimationDesc::EntityAnimationDesc& sCurrentAnim(msAnimation.asEntityAnimations[nAnim]);                         
				CEntity* pAnimEntity =  inpRootNode->FindChildEntityWithName(sCurrentAnim.strTargetPath);
				
				if (pAnimEntity)
					outEntityAnimList.push_back(pAnimController->CreateAnimation(sCurrentAnim.strName, pAnimEntity));
			}
            
            return true;
        }
        
        bool CSceneAnimation::PlayAnimation(const EntityPtr& inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr>& outEntityAnimList, AnimationPlayMode inePlayMode, InterpolationType ineInterpType) const
        {
            return PlayAnimation(inpRootNode.get(), outEntityAnimList, inePlayMode, ineInterpType);
        }
        
        bool CSceneAnimation::PlayAnimation(CEntity* inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr>& outEntityAnimList, AnimationPlayMode inePlayMode, InterpolationType ineInterpType) const
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
        
        void CSceneAnimation::SetSceneAnimDesc(const CSceneAnimationDesc &inAnimation)
        {
            msAnimation = inAnimation;
        }
        
    }
}
