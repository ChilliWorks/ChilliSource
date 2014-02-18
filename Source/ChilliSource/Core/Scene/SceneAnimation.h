//
//  SceneAnimation.h
//  moFlo
//
//  Scene Animation is a collection of entity animations 
//  which can be applied to a root entity
//
//  Created by Andrew Glass on 19/04/2012.
//  Copyright (c) 2012 Tag Games. All rights reserved.
//

#ifndef MoshiMonsters_SceneAnimation_h
#define MoshiMonsters_SceneAnimation_h

#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Scene/ForwardDeclarations.h>
#include <ChilliSource/Core/Entity/EntityAnimation.h>

namespace  moFlo
{
    namespace Core 
    {
        struct CSceneAnimationDesc
        {
            //--------------------------------------------------------------
            /// LoadFromBinaryFile
            ///
            /// Load in animation data from animation file
            ///
            /// @param Storage location
            /// @param Path to file
            /// @return Success
            //---------------------------------------------------------------
            bool LoadFromBinaryFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath);
            //--------------------------------------------------------------
            /// SaveToBinaryFile
            ///
            /// Save animation data to animation file
            ///
            /// @param Storage location
            /// @param Path to file
            /// @return Success
            //---------------------------------------------------------------
            void SaveToBinaryFile(STORAGE_LOCATION ineStorageLocation, const std::string & inFilePath);
            struct EntityAnimationDesc
			{
				std::string strTargetPath;
				std::string strName;
                u32 udwKeyframeCount;
				SHARED_PTR<f32> afKeyframeTimes;
				SHARED_PTR<EntityTransform>	aKeyframeValues;
                
				void FromBinaryStream(const Core::FileStreamPtr & inStream);
				void ToBinaryStream(const Core::FileStreamPtr & inStream);
			};
            
            DYNAMIC_ARRAY<EntityAnimationDesc>	asEntityAnimations;
        };
        
        class CSceneAnimation : public moFlo::Core::IResource
        {
        public:
            DECLARE_NAMED_INTERFACE(CSceneAnimation);
            bool IsA(InterfaceIDType inInterface) const;
            
            //--------------------------------------------------------------
            /// ApplyAnimationToScene
            ///
            /// Apply scene animation to Entity and its children
            /// returning list of animation instances
            ///
            /// @param Root entity to apply animations to 
            /// @param Output, instances animations in this scene animation
            /// @return Success
            //---------------------------------------------------------------
            bool ApplyAnimationToScene(const EntityPtr& inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr>& outEntityAnimList) const;
            bool ApplyAnimationToScene(CEntity* inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr> &outEntityAnimList) const;
            
            //--------------------------------------------------------------
            /// PlayAnimation
            ///
            /// Apply (and play) scene animation to Entity and its children
            /// returning list of animation instances
            ///
            /// @param Root entity to apply animations to 
            /// @param Output, instances animations in this scene animation
            /// @param Playback mode
            /// @return Success
            //---------------------------------------------------------------
            bool PlayAnimation(const EntityPtr& inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr>& outEntityAnimList, moFlo::AnimationPlayMode inePlayMode = moFlo::ANIM_PLAYONCE, InterpolationType ineInterType = INTERPOLATION_LINEAR) const;
            bool PlayAnimation(CEntity* inpRootNode, DYNAMIC_ARRAY<EntityAnimationPtr>& outEntityAnimList, moFlo::AnimationPlayMode inePlayMode = moFlo::ANIM_PLAYONCE, InterpolationType ineInterType = INTERPOLATION_LINEAR) const;
            
            //--------------------------------------------------------------
            /// SetSceneAnimDesc
            ///
            /// Set resources animation data
            ///
            /// @param Scene animation data
            //---------------------------------------------------------------
            void SetSceneAnimDesc(const CSceneAnimationDesc& inAnimation);
            
        protected:
            CSceneAnimationDesc msAnimation;
        };
    }
}
#endif
