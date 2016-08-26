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

#ifndef _CHILLISOURCE_RENDERING_ANIMATED_MESH_COMPONENT_H_
#define _CHILLISOURCE_RENDERING_ANIMATED_MESH_COMPONENT_H_

#include <ChilliSource/ChilliSource.h>
#include <ChilliSource/Core/Event/Event.h>
#include <ChilliSource/Core/File/FileSystem.h>
#include <ChilliSource/Core/Volume/VolumeComponent.h>
#include <ChilliSource/Rendering/Model/SkinnedAnimationGroup.h>
#include <ChilliSource/Rendering/Model/Model.h>

#include <functional>

namespace ChilliSource
{
    /// A component for playback of skinned animations. Animations can be set either through
    /// the constructor or by using SetAnimation(). An animation must always be active (though not
    /// necessarily playing) on a AnimatedModelComponent, otherwise it will assert.
    ///
    /// Animations can be played back either looping on set to "once". Looping animations will loop
    /// indefinitely and send the animation looped event every time it re-starts from the
    /// beginning. A playback type of "once" will result in the animation playing through to the
    /// end and the remaining on the final frame indefinitely. The animation completion event
    /// will also be sent.
    ///
    /// At any time while an animation is playing a new animation be faded into by calling FadeTo(),
    /// supplying the new animation that should be played.
    ///
    /// Although basic animation can be handled by setting a single animation at a time, more
    /// complex animations can be handled though animation blending. Instead of setting animations
    /// using SetAnimation(), models using blended animation have multiple animations attached using
    /// AttachAnimation(), which allows the animation to be attached to a specific point on the
    /// "Blend line". The current blendline value can be altered at anytime and determines what
    /// portion of each animation is blended.
    ///
    /// When using Animation blending FadeOut() can be used instead of FadeTo(), in which a new
    /// animation or series of animations must be attached immediately.
    ///
    /// This is not thread-safe and should only be accessed from the main thread.
    ///
    class AnimatedModelComponent final : public VolumeComponent
    {
    public:
        CS_DECLARE_NAMEDTYPE(AnimatedModelComponent);
        
        /// An enum describing the different types of playback that are available.
        ///
        /// Looping animations will loop indefinitely and send the animation looped event every time it re-starts
        /// from the beginning.
        ///
        /// A playback type of "once" will result in the animation playing through to the end and the remaining on
        /// the final frame indefinitely. The animation completion event will also be sent.
        ///
        enum class PlaybackType
        {
            k_once,
            k_looping
        };
        
        /// A delegate which is called whenever the animation changes.
        ///
        /// @param animatedModelComponent
        ///     The animated model component which has changed.
        ///
        using AnimationChangedDelegate = std::function<void(AnimatedModelComponent* animatedModelComponent)>;

        /// A delegate which is called when animations with a playback type of "once" finish animating.
        ///
        /// @param animatedModelComponent
        ///     The animated model component which has completed.
        ///
        typedef std::function<void(AnimatedModelComponent*)> AnimationCompletionDelegate;

        /// A delegate which is called when animations with a playback type of "looping" finish a single
        /// playthrough of the animation and start again.
        ///
        /// @param animatedModelComponent
        ///     The animated model component which has completed.
        ///
        typedef std::function<void(AnimatedModelComponent*)> AnimationLoopedDelegate;
        
        /// Creates a new animated model component with the given model, material and animation.
        ///
        /// @param model
        ///     The model which should be rendered. Must have animation data or this will assert.
        /// @param material
        ///     The material that will be applied to all meshes in the model.
        /// @param skinnedAnimation
        ///     The initial animation of the model.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        AnimatedModelComponent(const ModelCSPtr& model, const MaterialCSPtr& material, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept;
        
        /// Creates a new animated model component with the given model, material and animation.
        ///
        /// @param model
        ///     The model which should be rendered. Must have animation data or this will assert.
        /// @param materials
        ///     The material for each mesh in the model. Must have the same number of elements as
        ///     there are meshes in the given model.
        /// @param skinnedAnimation
        ///     The initial animation of the model.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        AnimatedModelComponent(const ModelCSPtr& in_model, const std::vector<MaterialCSPtr>& materials, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept;

        /// Allows querying of whether or not this component implements the interface described
        /// by the given interface Id. Typically this is not called directly as the templated
        /// equivalent IsA<Interface>() is preferred.
        ///
        /// @param interfaceId
        ///     The Id of the interface.
        ///
        /// @return Whether or not the interface is implemented.
        ///
        bool IsA(InterfaceIDType interfaceId) const noexcept override;

        /// Calculates and returns the world space AABB of the model. This is cached and only
        /// recomputed when required.
        ///
        /// @return The world space axis-aligned bounding box
        ///
        const AABB& GetAABB() noexcept override;
        
        /// Calculates and returns the world space OOBB of the model. This is cached and only
        /// recomputed when required.
        ///
        /// @return The world space OOBB
        ///
        const OOBB& GetOOBB() noexcept override;
        
        /// Calculates and returns the world space bounding sphere of the model. This is cached
        /// and only recomputed when required.
        ///
        /// @return The world space bounding sphere
        ///
        const Sphere& GetBoundingSphere() noexcept override;
        
        /// @return Whether or not the object should be rendered.
        ///
        bool IsVisible() const noexcept override { return m_isVisible; }
        
        /// @param isVisible
        ///     Whether or not the object should be rendered.
        ///
        void SetVisible(bool isVisible) noexcept { m_isVisible = isVisible; }
        
        /// @return The model that will be rendered.
        ///
        const ModelCSPtr& GetModel() const noexcept;
        
        /// Sets the model the component should use.
        ///
        /// The material currently applied to the first mesh will be applied to the entire model.
        ///
        /// The current animation will be invalidated meaning that a new animation must be attached
        /// prior to the next render snapshot or an assertion will occur.
        ///
        /// @param model
        ///     The model which should be used.
        ///
        void SetModel(const ModelCSPtr& model) noexcept;
        
        /// Sets the model the component should use. The given material will be applied to all meshes.
        ///
        /// The current animation will be invalidated meaning that a new animation must be attached
        /// prior to the next render snapshot or an assertion will occur.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param material
        ///     The material which should be used.
        ///
        void SetModel(const ModelCSPtr& model, const MaterialCSPtr& material) noexcept;
        
        /// Sets the model the component should use. The given materials will be applied to the
        /// respective meshes.
        ///
        /// The current animation will be invalidated meaning that a new animation must be attached
        /// prior to the next render snapshot or an assertion will occur.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param materials
        ///     The material for each mesh in the model. Must have the same number of elements as
        ///     there are meshes in the given model.
        ///
        void SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials) noexcept;
        
        /// Sets the model and animation the component should use.
        ///
        /// The material currently applied to the first mesh will be applied to the entire model.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param skinnedAnimation
        ///     The skinned animation that should be used.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        void SetModel(const ModelCSPtr& model, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept;
        
        /// Sets the model and animation the component should use. The given material will be applied
        /// to all meshes.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param material
        ///     The material which should be used.
        /// @param skinnedAnimation
        ///     The skinned animation that should be used.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        void SetModel(const ModelCSPtr& model, const MaterialCSPtr& material, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept;
        
        /// Sets the model and animation the component should use. The given materials will be applied
        /// to the respective meshes.
        ///
        /// @param model
        ///     The model which should be used.
        /// @param materials
        ///     The material for each mesh in the model. Must have the same number of elements as
        ///     there are meshes in the given model.
        /// @param skinnedAnimation
        ///     The skinned animation that should be used.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        void SetModel(const ModelCSPtr& model, const std::vector<MaterialCSPtr>& materials, const SkinnedAnimationCSPtr& skinnedAnimation, PlaybackType playbackType) noexcept;
        
        /// Get the material of a single mesh.
        ///
        /// @param meshIndex
        ///     Index to the mesh
        ///
        /// @return Handle to material
        ///
        const MaterialCSPtr& GetMaterialForMesh(u32 meshIndex) const noexcept;
        
        /// Get the material of a single mesh.
        ///
        /// @param meshName
        ///     The name of the mesh.
        ///
        /// @return Handle to material
        ///
        const MaterialCSPtr& GetMaterialForMesh(const std::string& meshName) const noexcept;
        
        /// Set the material that the model will use. Applies the material to all meshes.
        ///
        /// @param material
        ///     The material that should be used.
        ///
        void SetMaterial(const MaterialCSPtr& material) noexcept;
        
        /// Sets the material for a single mesh.
        ///
        /// @param material
        ///     The material that should be used.
        /// @param meshIndex
        ///     The index of the mesh.
        ///
        void SetMaterialForMesh(const MaterialCSPtr& material, u32 meshIndex) noexcept;
        
        /// Sets the material for a single mesh.
        ///
        /// @param material
        ///     The material that should be used.
        /// @param meshName
        ///     The name of the mesh
        ///
        void SetMaterialForMesh(const MaterialCSPtr& material, const std::string& meshName) noexcept;
        
        /// @return Whether the render component casts shadows
        ///
        bool IsShadowCastingEnabled() const noexcept { return m_shadowCastingEnabled; };
        
        /// @param enabled
        ///     Whether the render component casts shadows
        ///
        void SetShadowCastingEnabled(bool enabled) noexcept { m_shadowCastingEnabled = enabled; };
        
        /// @return The list of all active animations.
        ///
        std::vector<SkinnedAnimationCSPtr> GetAnimations() const noexcept;
        
        /// Sets the currently playing animation with the given playback type.
        ///
        /// This will change the animation immediately; if fade is required, then FadeTo() or FadeOut()
        /// should be used instead.
        ///
        /// The animation will replace all animations currently on the blendline and attach the
        /// animation at 0.0. If other blending behaviour is desired then AttachAnimation() should be
        /// used instead.
        ///
        /// @param animation
        ///     The animation to change to.
        /// @param playbackType
        ///     The playback type that should be used.
        ///
        void SetAnimation(const SkinnedAnimationCSPtr& animation, PlaybackType playbackType) noexcept;
        
        /// Attached a new animation to the animation blend line. The animation will be attached to the
        /// active animation group at the given blendline position.
        ///
        /// This is used for animation blending; if blending isn't required then SetAnimation() should
        /// be used instead.
        ///
        /// @param animation
        ///     The animation that should be attached to the blendline.
        /// @param blendlinePosition
        ///     The position on the blendline that the animation should be attached.
        ///
        void AttachAnimation(const SkinnedAnimationCSPtr& animation, f32 blendlinePosition) noexcept;

        /// Removes the given animation from the active animation group. If the animation is not
        /// currently attached to the active animation group this will assert.
        ///
        /// @param animation
        ///     The animation that should be removed.
        ///
        void DetatchAnimation(const SkinnedAnimationCSPtr& animation) noexcept;
        
        /// Fades the currently playing animation to the given animation.
        ///
        /// The new animation will be attached to the active blendline at 0.0; if different blending
        /// behaviour is desired then FadeOut() should be used.
        ///
        /// @param animation
        ///     The animation to fade to.
        /// @param playbackType
        ///     The playback type that should be used for the new animation.
        /// @param fadeType
        ///     The type of blending that should be used during the fade.
        /// @param fadeOutTime
        ///     The length of time that the fade should take in seconds.
        ///
        void FadeTo(const SkinnedAnimationCSPtr& animation, PlaybackType playbackType, AnimationBlendType fadeType, f32 fadeOutTime) noexcept;
        
        /// Fades out the active animation group, replacing it with a new group. This new group will
        /// have no animations attached to it, meaning that AttachAnimation must be called prior to
        /// the next render snapshot or an assertion will occur.
        ///
        /// If only basic animation is required, the convenience method FadeTo() should be used
        /// instead.
        ///
        /// @param fadeType
        ///     The type of blending that should be used during the fade.
        /// @param fadeOutTime
        ///     The length of time that the fade should take in seconds.
        ///
        void FadeOut(AnimationBlendType fadeType, f32 fadeOutTime) noexcept;
        
        /// Clears the component of all animations--both active and fading--and creates a new
        /// active animation group. This new group will have no animations attached to it, meaning
        /// that AttachAnimation must be called prior to the next render snapshot or an assertion
        /// will occur.
        ///
        void ClearAnimations() noexcept;
        
        /// @return the current position in the blendline.
        ///
        f32 GetBlendlinePosition() const noexcept { return m_blendlinePosition; };
        
        /// Sets the current position on the blendline.
        ///
        /// @param The blendline position.
        ///
        void SetBlendlinePosition(f32 blendlinePosition) noexcept;
        
        /// @return The current playback type.
        ///
        PlaybackType GetPlaybackType() const noexcept { return m_playbackType; }
        
        /// Sets the current playback type, looping, etc.
        ///
        /// @param playbackType
        ///     The playback type.
        ///
        void SetPlaybackType(PlaybackType playbackType) noexcept;
        
        /// @return The multiplier that is applied to the playback speed.
        ///
        f32 GetPlaybackSpeedMultiplier() const noexcept { return m_playbackSpeedMultiplier; }

        /// Sets the current playback speed multiplier.
        ///
        /// @param speedMultiplier
        ///     The playback speed multiplier.
        ///
        void SetPlaybackSpeedMultiplier(f32 speedMultiplier) noexcept { m_playbackSpeedMultiplier = speedMultiplier; }
        
        /// @return the number of seconds into the animation.
        ///
        f32 GetPlaybackPosition() const noexcept { return m_playbackPosition; }
        
        /// Sets the current position through the animation.
        ///
        /// @param position
        ///     The position
        ///
        void SetPlaybackPosition(f32 position) noexcept;
        
        /// @return distance through the animation normalised to between 0 and 1.
        ///
        f32 GetPlaybackPositionNormalised() const noexcept { return GetPlaybackPosition() / GetAnimationLength(); }
        
        /// Sets the current position through the animation normalised to between 0.0 and 1.0.
        ///
        /// @param position
        ///     The position normalised.
        ///
        void SetPlaybackPositionNormalised(f32 position) noexcept;
        
        /// @return the blend type used for all animation blending.
        ///
        AnimationBlendType GetBlendType() const noexcept { return m_animationBlendType; }
        
        /// Sets the type of blending to be used when blending animations.
        ///
        /// @param blendType
        ///     The blend type.
        ///
        void SetBlendType(AnimationBlendType blendType) noexcept { m_animationBlendType = blendType; }
        
        /// @return The animation length in seconds.
        ///
        f32 GetAnimationLength() const noexcept;
        
        /// @return whether or not the animation has finished.
        ///
        bool HasFinished() const noexcept { return m_finished; }
        
        /// Attached an entity to a node on the animated models skeleton.
        ///
        /// @param entity
        ///     The entity.
        /// @param nodeName
        ///     The name of the node to attach to.
        ////
        void AttachEntity(const EntitySPtr& entity, const std::string& nodeName) noexcept;
        
        /// Removes an entity that has previously been attached to the animated models skeleton.
        ///
        /// @param The entity.
        ///
        void DetatchEntity(const Entity* inpEntity) noexcept;
        
        /// Removes all entities that have previously been attached to the animated models skeleton.
        ///
        void DetatchAllEntities() noexcept;
        
        /// @param The animation changed event. This is invoked every time the current animation
        /// group changes, e.g Fade Out or Clear Animations is called.
        ///
        Event<AnimationChangedDelegate>& GetAnimationChangedEvent() noexcept { return m_animationChangedEvent; }

        /// @param The animation completion event. This is invoked whenever a non-looping animation
        /// reaches its end.
        ///
        Event<AnimationCompletionDelegate>& GetAnimationCompletionEvent() noexcept { return m_animationCompletionEvent; }

        /// @param The animation looped event. This is invoked every time an animation loops back
        ///     to the start of an animation.
        ///
        Event<AnimationLoopedDelegate>& GetAnimationLoopedEvent() noexcept { return m_animationLoopedEvent; }
        
    private:
        /// Updates the animation, rebuilding the animation matrices.
        ///
        /// @param deltaTime
        ///     The delta time.
        ///
        void UpdateAnimation(f32 deltaTime) noexcept;

        /// Updates the animation timer.
        ///
        /// @param deltaTime
        ///     The delta time.
        ///
        void UpdateAnimationTimer(f32 deltaTime) noexcept;

        /// Update the transforms of all entities attached to this animated model components skeleton.
        ///
        void UpdateAttachedEntities() noexcept;

        /// Resets the component back to a state where it is ready to start a new animation.
        ///
        void Reset() noexcept;
        
        /// Triggered when the component is added to the scene.
        ///
        void OnAddedToScene() noexcept override;

        /// Updates the animation.
        ///
        /// @param deltaTime
        ///     The delta time.
        ///
        void OnUpdate(f32 deltaTime) noexcept override;
        
        /// Called during the render snapshot phase. Adds render objects to the scene describing
        /// the model.
        ///
        /// @param renderSnapshot
        ///     The render snapshot.
        /// @param frameAllocator - Use this to allocate any memory
        /// for this render frame
        ///
        void OnRenderSnapshot(RenderSnapshot& renderSnapshot, IAllocator* frameAllocator) noexcept override;
        
        /// Triggered when the component is removed to the scene.
        ///
        void OnRemovedFromScene() noexcept override;
        
    private:
        typedef std::vector<std::pair<EntityWPtr, s32> > AttachedEntityList;
        
        AttachedEntityList m_attachedEntities;
        ModelCSPtr m_model;
        std::vector<MaterialCSPtr> m_materials;
        SkinnedAnimationGroupSPtr m_activeAnimationGroup;
        SkinnedAnimationGroupSPtr m_fadingAnimationGroup;
        f32 m_playbackPosition = 0.0f;
        f32 m_playbackSpeedMultiplier = 1.0f;
        f32 m_blendlinePosition = 0.0f;
        PlaybackType m_playbackType;
        AnimationBlendType m_animationBlendType = AnimationBlendType::k_linear;
        AnimationBlendType m_fadeType = AnimationBlendType::k_linear;
        f32 m_fadeTimer = 0.0f;
        f32 m_maxFadeTime = 1.0f;
        f32 m_fadePlaybackPosition = 0.0f;
        f32 m_fadeBlendlinePosition = 0.0f;
        bool m_finished = false;
        bool m_animationDataDirty = true;
        Event<AnimationCompletionDelegate> m_animationCompletionEvent;
        Event<AnimationLoopedDelegate> m_animationLoopedEvent;
        Event<AnimationChangedDelegate> m_animationChangedEvent;
        
        AABB m_aabb;
        OOBB m_oobb;
        Sphere m_boundingSphere;
        bool m_shadowCastingEnabled = true;
        bool m_isVisible = true;
    };
}

#endif
