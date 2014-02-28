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

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Resource/Resource.h>
#include <ChilliSource/Core/Entity/EntityAnimation.h>

namespace ChilliSource
{
    namespace Core 
    {
        struct SceneAnimationDesc
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
            bool LoadFromBinaryFile(StorageLocation ineStorageLocation, const std::string & inFilePath);
            //--------------------------------------------------------------
            /// SaveToBinaryFile
            ///
            /// Save animation data to animation file
            ///
            /// @param Storage location
            /// @param Path to file
            /// @return Success
            //---------------------------------------------------------------
            void SaveToBinaryFile(StorageLocation ineStorageLocation, const std::string & inFilePath);
            struct EntityAnimationDesc
			{
				std::string strTargetPath;
				std::string strName;
                u32 udwKeyframeCount;
				std::shared_ptr<f32> afKeyframeTimes;
				std::shared_ptr<EntityTransform>	aKeyframeValues;
                
				void FromBinaryStream(const Core::FileStreamSPtr & inStream);
				void ToBinaryStream(const Core::FileStreamSPtr & inStream);
			};
            
            std::vector<EntityAnimationDesc>	asEntityAnimations;
        };
        
        class SceneAnimation : public ChilliSource::Core::Resource
        {
        public:
            CS_DECLARE_NAMEDTYPE(SceneAnimation);
            bool IsA(InterfaceIDType inInterface) const override;
            
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
            bool ApplyAnimationToScene(const EntitySPtr& inpRootNode, std::vector<EntityAnimationSPtr>& outEntityAnimList) const;
            bool ApplyAnimationToScene(Entity* inpRootNode, std::vector<EntityAnimationSPtr> &outEntityAnimList) const;
            
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
            bool PlayAnimation(const EntitySPtr& inpRootNode, std::vector<EntityAnimationSPtr>& outEntityAnimList, AnimationPlayMode inePlayMode = AnimationPlayMode::k_once, InterpolationType ineInterType = InterpolationType::k_linear) const;
            bool PlayAnimation(Entity* inpRootNode, std::vector<EntityAnimationSPtr>& outEntityAnimList, AnimationPlayMode inePlayMode = AnimationPlayMode::k_once, InterpolationType ineInterType = InterpolationType::k_linear) const;
            
            //--------------------------------------------------------------
            /// SetSceneAnimDesc
            ///
            /// Set resources animation data
            ///
            /// @param Scene animation data
            //---------------------------------------------------------------
            void SetSceneAnimDesc(const SceneAnimationDesc& inAnimation);
            
        protected:
            SceneAnimationDesc msAnimation;
        };
    }
}
#endif
