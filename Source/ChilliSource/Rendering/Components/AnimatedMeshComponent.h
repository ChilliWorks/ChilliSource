/*
 *  AnimatedMeshComponent.h
 *  MoFlowSkeleton
 *
 *  Created by Ian Copland on 17/10/2011.
 *  Copyright 2011 Tag Games Ltd. All rights reserved.
 *
 */

#ifndef _MO_FLO_RENDERING_ANIMATED_MESH_COMPONENT_H_
#define _MO_FLO_RENDERING_ANIMATED_MESH_COMPONENT_H_

#include <ChilliSource/Rendering/Components/RenderComponent.h>
#include <ChilliSource/Rendering/SkinnedAnimationGroup.h>
#include <ChilliSource/Rendering/Mesh.h>
#include <ChilliSource/Core/ForwardDeclarations.h>
#include <ChilliSource/Core/FileIO/FileSystem.h>
#include <ChilliSource/Core/FastDelegate.h>
#include <ChilliSource/Core/GenericEvent.h>

namespace moFlo
{
	namespace Rendering
	{
        //===============================================================
		/// Animation Playback Type
		///
		/// Describes the different types of animation playback.
		//===============================================================
        namespace AnimationPlaybackType
        {
            enum ENUM
            {
                ONCE,
                LOOPING
            };
        }
        //===============================================================
		/// Events
		//===============================================================
        typedef fastdelegate::FastDelegate1<CAnimatedMeshComponent*> AnimationChangedDelegate;
        typedef CEvent1<AnimationChangedDelegate> AnimationChangedEvent;
        typedef fastdelegate::FastDelegate1<CAnimatedMeshComponent*> AnimationCompletionDelegate;
        typedef CEvent1<AnimationCompletionDelegate> AnimationCompletionEvent;
        typedef fastdelegate::FastDelegate1<CAnimatedMeshComponent*> AnimationLoopedDelegate;
        typedef CEvent1<AnimationLoopedDelegate> AnimationLoopedEvent;
		//===============================================================
		/// Animated Mesh component
		///
		/// An animated mesh component. This defines a 3D mesh that can
		/// be manipulated, textured and animated.
		//===============================================================
		class CAnimatedMeshComponent : public IRenderComponent
		{
		public:
			DECLARE_NAMED_INTERFACE(CAnimatedMeshComponent);
			
			CAnimatedMeshComponent();
			~CAnimatedMeshComponent();
			//----------------------------------------------------------
			/// Is A
			///
			/// Returns if it is of the type given
			/// @param Comparison Type
			/// @return Whether the class matches the comparison type
			//----------------------------------------------------------
			bool IsA(Core::InterfaceIDType inInterfaceID) const;
			//----------------------------------------------------
			/// Get Axis Aligned Bounding Box
			///
			/// All render components have a box used for culling
			/// and coarse intersections. This is cached and 
			/// recomputed when required.
			/// @return Axis aligned bounding box
			//----------------------------------------------------
			const Core::AABB& GetAABB();
			//----------------------------------------------------
			/// Get Object Oriented Bounding Box
			///
			/// All render objects have an OOBB for
			/// picking. This is cached and 
			/// recomputed when required.
			/// @return OOBB
			//----------------------------------------------------
			const Core::OOBB& GetOOBB();
			//----------------------------------------------------
			/// Get Bounding Sphere
			///
			/// All render objects have an bounding sphere for
			/// culling. This is cached and 
			/// recomputed when required.
			/// @return bounding sphere
			//----------------------------------------------------
			const Core::Sphere& GetBoundingSphere();
			//-----------------------------------------------------------
			/// Is Transparent
			///
			/// Returns whether or not this component has any transparency
			///
			/// @return whether or not this has transparency
			//-----------------------------------------------------------
			bool IsTransparent();
			//-----------------------------------------------------------
			/// Set Material
			///
			/// Set the material that the mesh will use. Applies the material
			/// To all submeshes
			///
			/// @param Handle to material
			//-----------------------------------------------------------
			void SetMaterial(const MaterialPtr& inpMaterial);
			//-----------------------------------------------------------
			/// Set Material For Sub Mesh
			///
			/// Set the material that one sub mesh will use.
			///
			/// @param Handle to material
			/// @Param Index to the submesh
			//-----------------------------------------------------------
			void SetMaterialForSubMesh(const MaterialPtr& inpMaterial, u32 indwSubMeshIndex);
            //-----------------------------------------------------------
			/// Set Material For Sub Mesh
			///
			/// Set the material that one sub mesh will use.
			///
			/// @param Handle to material
			/// @param The name of the submesh.
			//-----------------------------------------------------------
			void SetMaterialForSubMesh(const MaterialPtr& inpMaterial, const std::string& instrSubMeshName);
			//-----------------------------------------------------------
			/// Get Material Of Sub Mesh
			///
			/// Get the material of a single sub mesh.
			///
			/// @param Index to the sub mesh
			/// @return Handle to material
			//-----------------------------------------------------------
			const MaterialPtr GetMaterialOfSubMesh(u32 indwSubMeshIndex) const;
            //-----------------------------------------------------------
			/// Get Material Of Sub Mesh
			///
			/// Get the material of a single sub mesh.
			///
			/// @param The name of the submesh.
			/// @return Handle to material
			//-----------------------------------------------------------
			MaterialPtr GetMaterialOfSubMesh(const std::string& instrSubMeshName) const;
			//----------------------------------------------------------
			/// Attach Mesh
			///
			/// Attach a mesh to this component
			/// @param Mesh object
			//----------------------------------------------------------
			void AttachMesh(const MeshPtr& inpModel);
            //----------------------------------------------------------
            /// Attach Mesh
            ///
            /// Attach a mesh to this component but uses the given 
            /// material
            /// @param Mesh object
            //----------------------------------------------------------
            void AttachMesh(const MeshPtr& inpModel, const MaterialPtr& inpMaterial);
            //----------------------------------------------------------
            /// Get Mesh
            ///
            /// @return The components internal mesh
            //----------------------------------------------------------
            const MeshPtr& GetMesh() const;
			//----------------------------------------------------------
			/// Attach Animation
			///
			/// Attaches an animation to the active animation group.
            /// The active animation group must have at least one 
            /// animation in it for the mesh to render.
			///
            /// @param The storage location from which to load.
			/// @param the animation filename
            /// @param the position of the animation on the blendline.
			//----------------------------------------------------------
			void AttachAnimation(Core::STORAGE_LOCATION ineStorageLocation, const std::string& instrAnimation, f32 infBlendlinePosition = 0.0f);
            //----------------------------------------------------------
			/// Attach Animation
			///
			/// Attaches an animation to the active animation group.
            /// The active animation group must have at least one 
            /// animation in it for the mesh to render.
			///
            /// @param The animation pointer.
            /// @param the position of the animation on the blendline.
			//----------------------------------------------------------
			void AttachAnimation(const SkinnedAnimationPtr& inpAnimation, f32 infBlendlinePosition = 0.0f);
            //----------------------------------------------------------
			/// Detatch Animation
			///
			/// Removes an animation from the active animation group.
			///
            /// @param The animation pointer.
            /// @param the position of the animation on the blendline.
			//----------------------------------------------------------
			void DetatchAnimation(const SkinnedAnimationPtr& inpAnimation);
            //----------------------------------------------------------
			/// Get Animations
            ///
            /// @param OUT: A list of all attached animations.
			//----------------------------------------------------------
            void GetAnimations(DYNAMIC_ARRAY<SkinnedAnimationPtr>& outapSkinnedAnimationList);
            //----------------------------------------------------------
			/// Fade Out
			///
			/// Fades out the active animation group. The active group
            /// will become the "fading" group while a new animation
            /// group will replace it as "active". This new group will
            /// need to have at least one animation attached to it 
            /// before the mesh can be rendered.
			///
            /// @param The animation fade type.
            /// @param the time to fade out over.
			//----------------------------------------------------------
			void FadeOut(AnimationBlendType::ENUM ineFadeType, f32 infFadeOutTime);
            //----------------------------------------------------------
			/// Clear Animations
			///
			/// Clears the content of all animation groups. The
            /// mesh will not be able to render until at least one 
            /// animation has been added again to the active animation
            /// group.
			//----------------------------------------------------------
			void ClearAnimations();
            //----------------------------------------------------------
			/// Attach Entity
			///
			/// Attached an entity to a node on the animated meshes
            /// skeleton.
			///
            /// @param The entity.
            /// @param the name of the node to attach to.
			//----------------------------------------------------------
			void AttachEntity(const Core::EntityPtr& inpEntity, const std::string& instrNodeName);
            //----------------------------------------------------------
			/// Detatch Entity
			///
			/// Removes an entity that has previously been attached to
            /// the animated meshes skeleton.
			///
            /// @param The entity.
			//----------------------------------------------------------
			void DetatchEntity(Core::CEntity* inpEntity);
            //----------------------------------------------------------
			/// Detatch All Entities
			///
			/// Removes all entities that have previously been attached to
            /// the animated meshes skeleton.
			//----------------------------------------------------------
			void DetatchAllEntities();
            //----------------------------------------------------------
			/// Set Blendline Position
			///
			/// Sets the current position on the blendline.
            ///
            /// @param The blendline position.
			//----------------------------------------------------------
			void SetBlendlinePosition(f32 infBlendlinePosition);
            //----------------------------------------------------------
			/// Set Playback Type
			///
			/// Sets the current playback type, looping, etc.
            ///
            /// @param The playback type.
			//----------------------------------------------------------
			void SetPlaybackType(AnimationPlaybackType::ENUM inePlaybackType);
            //----------------------------------------------------------
			/// Set Playback Speed Multiplier
			///
			/// Sets the current playback speed multiplier.
            ///
            /// @param The playback speed multiplier.
			//----------------------------------------------------------
			void SetPlaybackSpeedMultiplier(f32 infSpeedMultiplier);
            //----------------------------------------------------------
			/// Set Playback Position
			///
			/// Sets the current position through the animation.
            ///
            /// @param The position
			//----------------------------------------------------------
			void SetPlaybackPosition(f32 infPosition);
            //----------------------------------------------------------
			/// Set Playback Position Normalised
			///
			/// Sets the current position through the animation normalised
            /// to between 0.0 and 1.0.
            ///
            /// @param The position normalised.
			//----------------------------------------------------------
			void SetPlaybackPositionNormalised(f32 infPosition);
            //----------------------------------------------------------
			/// Set Blend Type
			///
			/// Sets the type of blending to be used when blending
            /// animations.
            ///
            /// @param The blend type.
			//----------------------------------------------------------
			void SetBlendType(AnimationBlendType::ENUM ineBlendType);
            //----------------------------------------------------------
			/// Get Animation Changed Event
            ///
            /// @param The animation changed event. This is invoked every
            /// time the current animation group changes, e.g Fade Out
            /// or Clear Animations is called.
			//----------------------------------------------------------
			AnimationChangedEvent& GetAnimationChangedEvent();
            //----------------------------------------------------------
			/// Get Animation Completion Event
            ///
            /// @param The animation completion event. This is invoked
            /// whenever a non-looping animation reaches it's end.
			//----------------------------------------------------------
			AnimationCompletionEvent& GetAnimationCompletionEvent();
            //----------------------------------------------------------
			/// Get Animation Looped Event
            ///
            /// @param The animation looped event. This is invoked every
            /// Time an animation loops back to the start of an animation.
			//----------------------------------------------------------
			AnimationLoopedEvent& GetAnimationLoopedEvent();
            //----------------------------------------------------------
			/// Get Blendline Position
            ///
            /// @return the current position along the blendline.
			//----------------------------------------------------------
            f32 GetBlendlinePosition() const;
            //----------------------------------------------------------
			/// Get Playback Type
            ///
            /// @return the current playback type.
			//----------------------------------------------------------
            AnimationPlaybackType::ENUM GetPlaybackType() const;
            //----------------------------------------------------------
			/// Get Animation Length
            ///
            /// @return the animation length in seconds.
			//----------------------------------------------------------
            f32 GetAnimationLength();
            //----------------------------------------------------------
			/// Get Playback Speed Multiplier
            ///
            /// @return the multiplier that is curently applied to the
            ///         playback speed.
			//----------------------------------------------------------
            f32 GetPlaybackSpeedMultiplier() const;
            //----------------------------------------------------------
			/// Get Playback Position
            ///
            /// @return the number of seconds into the animation.
			//----------------------------------------------------------
            f32 GetPlaybackPosition() const;
            //----------------------------------------------------------
			/// get Playback Position Normalised
            ///
            /// @return distance through the animation normalised to
            ///         between 0 and 1.
			//----------------------------------------------------------
            f32 GetPlaybackPositionNormalised();
            //----------------------------------------------------------
			/// Get Blend Type
            ///
            /// @return the blend type used for all animation blending.
			//----------------------------------------------------------
            AnimationBlendType::ENUM GetBlendType() const;
            //----------------------------------------------------------
			/// Has Finished
            ///
            /// @return whether or not the animation has finished.
			//----------------------------------------------------------
            bool HasFinished() const;
            //----------------------------------------------------------
			/// Update
			///
			/// Updates the animation.
            ///
            /// @param The delta time.
			//----------------------------------------------------------
			void Update(f32 infDeltaTime);
		private:
			//----------------------------------------------------
			/// On Attached To Entity
			///
			/// Triggered when the component is attached to
			/// an entity
			//----------------------------------------------------
			void OnAttachedToEntity();
            //----------------------------------------------------
			/// On Detached From Entity
			///
			/// Triggered when the component is detached from
			/// an entity
			//----------------------------------------------------
			void OnDetachedFromEntity();
			//----------------------------------------------------------
			/// Render
			///
			/// Invoke render on objects mesh
            ///
            /// @param Render system visitor
            /// @param Active camera component
            /// @param The current shader pass.
			//----------------------------------------------------------
			void Render(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam, ShaderPass ineShaderPass);
            //-----------------------------------------------------
            /// Render Shadow Map
            ///
            /// Render the mesh to the shadow map
            ///
            /// @param Render system
            /// @param Active camera component
            //-----------------------------------------------------
            void RenderShadowMap(IRenderSystem* inpRenderSystem, CCameraComponent* inpCam);
            //----------------------------------------------------------
			/// Update Animation
			///
			/// Updates the animation, rebuilding the animation matrices.
            ///
            /// @param The delta time.
			//----------------------------------------------------------
			void UpdateAnimation(f32 infDeltaTime);
			//----------------------------------------------------------
			/// Update Animation Timer
			///
			/// Updates the animation timer.
            ///
            /// @param The delta time.
			//----------------------------------------------------------
			void UpdateAnimationTimer(f32 infDeltaTime);
            //----------------------------------------------------------
			/// Update Attached Entities
			///
			/// Update the transforms of all entities attached to this
            /// animated mesh components skeleton.
			//----------------------------------------------------------
			void UpdateAttachedEntities();
            //----------------------------------------------------------
			/// Reset
			///
			/// Resets the component back to a state where it is ready
            /// to start a new animation.
			//----------------------------------------------------------
			void Reset();
            //----------------------------------------------------
            /// Apply Default Materials
            ///
            /// Sets the materials to the defaults described in the
            /// mesh resource.
            //----------------------------------------------------
            void ApplyDefaultMaterials();
            
		private:
            typedef DYNAMIC_ARRAY<std::pair<moCore::EntityWeakPtr, s32> > AttachedEntityList;
            AttachedEntityList maAttachedEntities;
			MeshPtr mpModel;
			DYNAMIC_ARRAY<MaterialPtr> mMaterials;
			SkinnedAnimationGroupPtr mActiveAnimationGroup;
            SkinnedAnimationGroupPtr mFadingAnimationGroup;
			f32 mfPlaybackPosition;
			f32 mfPlaybackSpeedMultiplier;
            f32 mfBlendlinePosition;
            AnimationBlendType::ENUM meBlendType;
            AnimationPlaybackType::ENUM mePlaybackType;
            AnimationBlendType::ENUM meFadeType;
            f32 mfFadeTimer;
            f32 mfFadeMaxTime;
            f32 mfFadePlaybackPosition;
            f32 mfFadeBlendlinePosition;
            bool mbFinished;
            bool mbAnimationDataDirty;
            AnimationCompletionEvent mAnimationCompletionEvent;
            AnimationLoopedEvent mAnimationLoopedEvent;
            AnimationChangedEvent mAnimationChangedEvent;
            
            static MaterialPtr mspShadowMapMaterial;
		};
	}
}

#endif